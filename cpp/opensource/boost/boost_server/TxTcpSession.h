

#pragma once

#include <arpa/inet.h>
#include <queue>
#include <string>
#include <thread>

#include "TxTcpException.h"
#include "TxMessage.h"
#include "libdevcore/ThreadPool.h"
#include <libconfig/GlobalConfigure.h>
#include <libdevcore/Common.h>
#include <libdevcore/FixedHash.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using  namespace boost::asio;
using  namespace dev;
using boost::asio::ip::tcp;

namespace Tx_Join
{

class TxTcpSession
  : public std::enable_shared_from_this<TxTcpSession>
{
public:
  TxTcpSession(tcp::socket socket)
    : _socket(std::move(socket))
  {
     //memcpy(data_,"servr:",6);
  }

  void setThreadPool(ThreadPool::Ptr requestThreadPool)
  {
	  m_requestThreadPool = requestThreadPool;
	  
  }

  typedef std::shared_ptr<TxTcpSession> Ptr;

  virtual void setIOService(std::shared_ptr<boost::asio::io_service> IOService)
  {
	  _ioService = IOService;
  };

  virtual void setMessageHandler(
        std::function<void(TxTcpSession::Ptr, Tx_Join::TxJoinException, TxMessage::Ptr)>
            handler)
  {
        _messageHandler = handler;
  };

  
  void asyncSendMessage(TxMessage::Ptr request,  std::function<void(Tx_Join::TxJoinException, TxMessage::Ptr)> callback );
  
  void  run(); 
  

  void setMessageFactory(TxMessageFactory::Ptr messageFactory)
  {
	  _messageFactory = messageFactory;
  }

  void   writeBuffer(std::shared_ptr<bytes> buffer);

private:
  void   startWrite();
  void   startRead();
  void   onRead(const boost::system::error_code& error, std::size_t bytesTransferred);
  void   updateIdleTimer(); 
  void   onMessage(  TxMessage::Ptr message); 

  void   onWrite(const boost::system::error_code& error, std::shared_ptr<bytes>, size_t);

  void   onIdle(const boost::system::error_code& error);  
  void   disconnect(Tx_Join::TxJoinException e );


  tcp::socket _socket;




  
  bool _actived = false;

  
  std::string _host;
  int _port = 0;
  
  byte _recvBuffer[102400];
  enum { max_length = 102400 };
  bytes _recvProtocolBuffer;
  
  std::queue<std::shared_ptr<bytes>> _sendBufferList;
  bool _writing = false;
  
  std::shared_ptr<boost::asio::deadline_timer> _idleTimer;
  std::recursive_mutex _mutex;
  
  std::shared_ptr<boost::asio::io_service> _ioService;

  TxMessageFactory::Ptr  _messageFactory;
  std::function<void(TxTcpSession::Ptr, Tx_Join::TxJoinException, TxMessage::Ptr)>
        _messageHandler;

  ThreadPool::Ptr m_requestThreadPool;
  


  std::string m_clientType;
  // set idle time interval to 60s
  size_t _idleTime = 60;


  
};


} // Tx_Join

