


#include "TxTcpSession.h"

using namespace Tx_Join;


void TxTcpSession::run()
{
    try
    {
        if (!_actived)
        {
            _actived = true;

			_idleTimer = std::make_shared<boost::asio::deadline_timer>(*_ioService);
            //_idleTimer = std::make_shared<boost::asio::deadline_timer>(_sslSocket->get_io_service());    
            updateIdleTimer();

            startRead();
        }
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR) << "error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TxTcpSession::updateIdleTimer()
{
    if (!_actived)
    {
        TXJOIN_SESSION_LOG(ERROR) << "TxjoinSession inactived";

        return;
    }

    if (_idleTimer)
    {
        _idleTimer->expires_from_now(boost::posix_time::seconds(_idleTime));

        auto session = std::weak_ptr<TxTcpSession>(shared_from_this());
        _idleTimer->async_wait([session](const boost::system::error_code& error) {
            auto s = session.lock();
            if (s)
            {
                s->onIdle(error);
            }
        });
    }
}

void TxTcpSession::onIdle(const boost::system::error_code& error)
{
    try
    {
        if (!_actived)
        {
            TXJOIN_SESSION_LOG(ERROR) << "ChannelSession inactived";

            return;
        }

        if (error != boost::asio::error::operation_aborted)
        {
            TXJOIN_SESSION_LOG(ERROR) << "Idle connection, disconnect " << _host << ":" << _port;

            disconnect(TxJoinException(-1, "Idle connection, disconnect"));
        }
        else
        {
        }
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR)
            << "onIdle error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TxTcpSession::onRead(const boost::system::error_code& error, std::size_t bytesTransferred)
{
    try
    {
        if (!_actived)
        {
            TXJOIN_SESSION_LOG(ERROR) << "TxTcpSession inactived";

            return;
        }

        updateIdleTimer();

        if (!error)
        {
            _recvProtocolBuffer.insert(
                _recvProtocolBuffer.end(), _recvBuffer, _recvBuffer + bytesTransferred);

            while (true)
            {
                auto message = _messageFactory->buildMessage();

                ssize_t result =
                    message->decode(_recvProtocolBuffer.data(), _recvProtocolBuffer.size());

                if (result > 0)
                {
                    onMessage( message);

                    _recvProtocolBuffer.erase(
                        _recvProtocolBuffer.begin(), _recvProtocolBuffer.begin() + result);
                }
                else if (result == 0)
                {
                    startRead();

                    break;
                }
                else if (result < 0)
                {
                    TXJOIN_SESSION_LOG(ERROR) << "onRead Protocol parser error";

                    disconnect(TxJoinException(-1, "Protocol parser error, disconnect"));

                    break;
                }
            }
        }
        else
        {
            TXJOIN_SESSION_LOG(WARNING)
                << LOG_DESC("onRead failed") << LOG_KV("value", error.value())
                << LOG_KV("message", error.message());

            if (_actived)
            {
                disconnect(TxJoinException(-1, "Read failed, disconnect"));
            }
        }
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR)
            << "onRead error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}


void TxTcpSession::onMessage(  TxMessage::Ptr message)
{
    try
    {
        if (!_actived)
        {
            TXJOIN_SESSION_LOG(ERROR) << LOG_DESC("onMessage TxTcpSession inactived");

            return;
        }
        
        if (_messageHandler)
        {
            auto session = std::weak_ptr<Tx_Join::TxTcpSession>(shared_from_this());
            m_requestThreadPool->enqueue([session, message]() {
                auto s = session.lock();
                if (s && s->_messageHandler)
                {                    
                    s->_messageHandler(s, TxJoinException(0, ""),   message);
                }
            });
        }
        else
        {
            TXJOIN_SESSION_LOG(ERROR) << "MessageHandler empty";
        }
        
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR)
            << "onMessage error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void  TxTcpSession::startRead()
{
  auto self(shared_from_this());
  
  _socket.async_read_some(boost::asio::buffer(_recvBuffer, max_length),
	  [this, self](boost::system::error_code ec, std::size_t length)
	  {
		if (!ec)
		{
		  //BOOST_LOG_TRIVIAL(info) << "receive "<< socket_.remote_endpoint().address()<< " length "<< length;	
		  //BOOST_LOG_TRIVIAL(info) << "receive "<< data_ << " length "<< length;  
		  self->onRead(ec ,length);
		}
	  });
}


void TxTcpSession::startWrite()
{
    if (!_actived)
    {
        TXJOIN_SESSION_LOG(ERROR) << "startWrite TxTcpSession inactived";

        return;
    }

    if (_writing)
    {
        return;
    }

    if (!_sendBufferList.empty())
    {
        _writing = true;

        auto buffer = _sendBufferList.front();
        _sendBufferList.pop();

        auto session = std::weak_ptr<TxTcpSession>(shared_from_this());
        
        _ioService->post([session, buffer] {
            auto s = session.lock();
            if (s)
            {		  
                {
                    boost::asio::async_write(s->_socket,
                        boost::asio::buffer(buffer->data(), buffer->size()),
                        [=](const boost::system::error_code& error, size_t bytesTransferred) {
                            auto s = session.lock();
                            if (s)
                            {
                                //std::cout<< " ::boost::asio::async_write " << error <<  std::endl; 
                                s->onWrite(error, buffer, bytesTransferred);
                            }
                        });
                }
            }
        });
    }
    else
    {
        _writing = false;
    }
}

void TxTcpSession::writeBuffer(std::shared_ptr<bytes> buffer)
{
    try
    {
        std::lock_guard<std::recursive_mutex> lock(_mutex);

        _sendBufferList.push(buffer);

        startWrite();
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR)
            << "writeBuffer error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}


void TxTcpSession::onWrite(const boost::system::error_code& error, std::shared_ptr<bytes>, size_t)
{
    try
    {
        if (!_actived)
        {
            TXJOIN_SESSION_LOG(ERROR) << "TxTcpSession inactived";

            return;
        }

        std::lock_guard<std::recursive_mutex> lock(_mutex);
        
        updateIdleTimer();

        if (error)
        {
            TXJOIN_SESSION_LOG(ERROR)
                << LOG_DESC("Write error") << LOG_KV("message", error.message());

            disconnect(TxJoinException(-1, "Write error, disconnect"));
        }

        _writing = false;
        startWrite();
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR)
            << LOG_DESC("onWrite error") << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TxTcpSession::disconnect(Tx_Join::TxJoinException  )
{
    try
    {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        if (_actived)
        {
            _idleTimer->cancel();
            _actived = false;

            _socket.close() ; 

            TXJOIN_SESSION_LOG(DEBUG) << "Disconnected";
        }
    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(WARNING)
            << "Close error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TxTcpSession::asyncSendMessage(TxMessage::Ptr request,  std::function<void(Tx_Join::TxJoinException, TxMessage::Ptr)> callback )
{
    try
    {
        if (!_actived)
        {
            if (callback)
            {
                callback(TxJoinException(-3, "Session inactived"), TxMessage::Ptr());
            }

            return;
        }

        std::shared_ptr<bytes> p_buffer = std::make_shared<bytes>();
        request->encode(*p_buffer);
        
        writeBuffer(p_buffer);

    }
    catch (std::exception& e)
    {
        TXJOIN_SESSION_LOG(ERROR) << LOG_DESC("asyncSendMessage error")
                                   << LOG_KV("what", boost::diagnostic_information(e));
    }
}


