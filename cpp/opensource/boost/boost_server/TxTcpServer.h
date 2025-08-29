

#pragma once


#include "TxTcpSession.h"

using boost::asio::ip::tcp;

namespace Tx_Join
{

class TxTcpServer : public std::enable_shared_from_this<TxTcpServer>
{
public:
  TxTcpServer( )    
  {     

  }

  typedef std::shared_ptr<TxTcpServer> Ptr;

  TxMessageFactory::Ptr messageFactory() { return m_messageFactory; }
  void setMessageFactory(TxMessageFactory::Ptr messageFactory)
  {
      m_messageFactory = messageFactory;
  }

  void setBind(const std::string& host, int port)
  {
     m_listenHost = host;
     m_listenPort = port;
  };
	
  void setIOService(std::shared_ptr<boost::asio::io_service> ioService)
  {
        m_ioService = ioService;
  };

  void  run();
  
  void  stop();
  
  void startAccept();
  void setConnectionHandler(
	  std::function<void(Tx_Join::TxJoinException, TxTcpSession::Ptr)> handler)
  {
	   m_connectionHandler = handler;
  };


private:

  
  std::string m_listenHost = "";
  int m_listenPort = 0;
  
  std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
  ThreadPool::Ptr m_requestThreadPool;
  
  
  std::shared_ptr<boost::asio::io_service> m_ioService;


  std::function<void(Tx_Join::TxJoinException, TxTcpSession::Ptr)> m_connectionHandler;
  TxMessageFactory::Ptr m_messageFactory;

  
  std::shared_ptr<std::thread> m_serverThread;

   //boost::asio::ip::tcp::acceptor acceptor_;
};

}



