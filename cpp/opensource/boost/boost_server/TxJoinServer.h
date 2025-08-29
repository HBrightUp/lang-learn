/*
 * @CopyRight:
 * TIANHE-CHAIN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TIANHE-CHAIN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TIANHE-CHAIN.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 tianhe-dev contributors.
 */
/**
 * @file: TxJoinServer.h
 * @author: monan
 * @date: 2017
 *
 * @author alvin
 * @date 2018.11.5

 */

#pragma once

#include "TxMessage.h"         // for Message, Mess...
#include "TxTcpSession.h"         // for Message, Mess...
#include "TxTcpServer.h"         // for Message, Mess...
#include "libledger/DBInitializer.h"
#include "libledger/LedgerManager.h"  // for Message, Mess...
#include <boost/asio/io_service.hpp>  // for io_service
#include <atomic>                     // for atomic
#include <map>                        // for map
#include <mutex>                      // for mutex
#include <set>                        // for set
#include <string>                     // for string
#include <thread>
#include <utility>  // for swap, move
#include <vector>   // for vector


namespace Tx_Join
{

class TxJoinServer : public std::enable_shared_from_this<TxJoinServer>
{
public:
		
    typedef std::shared_ptr<TxJoinServer> Ptr;

    TxJoinServer(std::string listenAddr = "", int listenPort = 0)
      : _listenAddr(listenAddr),
        _listenPort(listenPort)
	{};

     ~TxJoinServer();
     bool StartListening() ;
     bool StopListening() ;



     void onClientRequest(Tx_Join::TxTcpSession::Ptr session, Tx_Join::TxJoinException e, Tx_Join::TxMessage::Ptr message);
     void  onDisconnect(Tx_Join::TxJoinException, Tx_Join::TxTcpSession::Ptr session); 

     void blockNotify(int16_t _groupID, int64_t _blockNumber);

    void setListenAddr(const std::string& listenAddr);

    void setListenPort(int listenPort);

    void onConnect(
        Tx_Join::TxJoinException e, Tx_Join::TxTcpSession::Ptr session);

    std::shared_ptr<Tx_Join::TxTcpServer> TxTcpServer() { return _server; }
    void setTxTcpServer(std::shared_ptr<Tx_Join::TxTcpServer> server)
    {
        _server = server;
    }

    void asyncPushChannelMessage(std::string topic, Tx_Join::TxMessage::Ptr message,
        std::function<void(Tx_Join::TxJoinException, Tx_Join::TxMessage::Ptr,
            Tx_Join::TxTcpSession::Ptr)>  callback);


   
	void  setLedger(std :: shared_ptr < dev::ledger::LedgerManager > ledger   )
	{
	    _ledger  =  ledger;
	}


private:

    void  onClientTran(Tx_Join::TxTcpSession::Ptr session ,    TxMessage::Ptr message) ;

    void  onClientPatchTran(Tx_Join::TxTcpSession::Ptr session ,    TxMessage::Ptr message ,int rpcflag);

    void  onClientGetBlockNo(Tx_Join::TxTcpSession::Ptr session ,    TxMessage::Ptr message ) ;

    void  onClientSimTran(Tx_Join::TxTcpSession::Ptr session ,    TxMessage::Ptr message );
    void  onClientPatchFpgaTran(Tx_Join::TxTcpSession::Ptr  ,    TxMessage::Ptr message ,int rpcflag );
    
    bool _running = false;

    std::string _listenAddr;
    int _listenPort;
    std::shared_ptr<boost::asio::io_service> _ioService;
    
    std::shared_ptr<Tx_Join::TxTcpServer> _server;

    std::map<int, Tx_Join::TxTcpSession::Ptr> _sessions;
    std::mutex _sessionMutex;
	std::shared_ptr<dev::ledger::LedgerManager >  _ledger;

    std::mutex _seqMutex;
    std::atomic<uint64_t> _txcount = {0};
    
    int _sessionCount = 1;

    int64_t   _currentBlockNo =0;

};


} //Tx_Join

