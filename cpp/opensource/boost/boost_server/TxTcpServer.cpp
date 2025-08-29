

#include "TxTcpServer.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>


#define TXJOIN_LOG(LEVEL) LOG(LEVEL) << "[TXJOIN]"


using namespace std;
using namespace Tx_Join;


void TxTcpServer::run()
{
	
    // ChannelReq process more request, should be larger
    m_requestThreadPool = std::make_shared<ThreadPool>("TxJoinReq", 8 );
    //m_responseThreadPool = std::make_shared<ThreadPool>("TxJoinResp", 1 );

    if (!m_listenHost.empty() && m_listenPort > 0)
    {
        m_acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(
            *m_ioService, boost::asio::ip::tcp::endpoint(
                      boost::asio::ip::make_address(m_listenHost), m_listenPort));

        boost::asio::socket_base::reuse_address optionReuseAddress(true);
        m_acceptor->set_option(optionReuseAddress);
    }

    m_serverThread = std::make_shared<std::thread>([=]() {
        pthread_setThreadName("TxTcpServer");

        while (m_acceptor && m_acceptor->is_open())
        {
            try
            {
                startAccept();
                m_ioService->run();
            }
            catch (std::exception& e)
            {
                TXJOIN_LOG(ERROR) << LOG_DESC("IO thread error")
                                   << LOG_KV("what", boost::diagnostic_information(e));
            }


            this_thread::sleep_for(std::chrono::milliseconds(1000));

            if (m_acceptor->is_open() && m_ioService->stopped())
            {
                TXJOIN_LOG(WARNING) << LOG_DESC("io_service reset");
                m_ioService->reset();
            }
        }
    });
}


void TxTcpServer::startAccept()
{
   m_acceptor->async_accept(
	  [this](boost::system::error_code ec, tcp::socket socket)
	  {
		if (!ec)
		{
		  TxTcpSession::Ptr session  =  std::make_shared<TxTcpSession>(std::move(socket));
		  session->setThreadPool(m_requestThreadPool);
		  session->setIOService(m_ioService);
          session->setMessageFactory(m_messageFactory);
		  if (m_connectionHandler)
          {
             m_connectionHandler( TxJoinException() ,session); 
          }					 		  
          //boost::system::error_code ec;
          //auto remoteEndpoint = session->Socket()->lowest_layer().remote_endpoint(ec);        
          //session->setHost(remoteEndpoint.address().to_string());
          //session->setPort(remoteEndpoint.port());
		
		  // 
		}

		startAccept();
	  });
}



void TxTcpServer::stop()
{
    try
    {
        TXJOIN_LOG(DEBUG) << LOG_DESC("Close acceptor");

        if (m_acceptor->is_open())
        {
            m_acceptor->cancel();
            m_acceptor->close();
        }
    }
    catch (std::exception& e)
    {
        TXJOIN_LOG(ERROR) << LOG_DESC("Close acceptor error")
                           << LOG_KV("what", boost::diagnostic_information(e));
    }

    try
    {
        TXJOIN_LOG(DEBUG) << LOG_DESC("Close ioService");
        m_ioService->stop();
    }
    catch (std::exception& e)
    {
        TXJOIN_LOG(ERROR) << LOG_DESC("Close ioService")
                           << LOG_KV("what", boost::diagnostic_information(e));
    }
    m_serverThread->join();
}



