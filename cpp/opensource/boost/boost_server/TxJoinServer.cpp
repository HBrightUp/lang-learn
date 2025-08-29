/*
 * @CopyRight:

 */
/**

 */

#include "TxJoinServer.h"
#include "libdevcore/Common.h"                  // for bytes, byte
#include "libethcore/TxsParallelParser.h"
#include <libethcore/ABI.h>
//#include <stdatomic.h>
#include <unistd.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/asio.hpp>

#include <boost/random.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>



using namespace std;
using namespace Tx_Join;

TxJoinServer::~TxJoinServer()
{
    StopListening();
}

bool TxJoinServer::StartListening()
{
    if (!_running)
    {
        TXJOIN_LOG(TRACE) << "Start TxJoinServer" << LOG_KV("Host", _listenAddr) << ":"
                           << _listenPort;

        std::function<void( Tx_Join::TxJoinException,  TxTcpSession::Ptr)> fp =
            std::bind(&TxJoinServer::onConnect, shared_from_this(), std::placeholders::_1,
                std::placeholders::_2);
        _server->setConnectionHandler(fp);

        _server->run();
        
        TXJOIN_LOG(INFO) << "TxJoinServer started";

        _running = true;
    }

    return true;
}

bool TxJoinServer::StopListening()
{
    if (_running)
    {
        _server->stop();
    }

    _running = false;

    return true;
}

void TxJoinServer::onConnect(  Tx_Join::TxJoinException e, TxTcpSession::Ptr session)
{
    if (e.errorCode() == 0)
    {
        std::function<void(Tx_Join::TxTcpSession::Ptr, Tx_Join::TxJoinException, Tx_Join::TxMessage::Ptr)>
            fp = std::bind(&Tx_Join::TxJoinServer::onClientRequest, this, std::placeholders::_1,
                    std::placeholders::_2, std::placeholders::_3);
        session->setMessageHandler(fp);
        
        session->run();
        TXJOIN_LOG(INFO) << "start receive";
    }
    else{

    }
}

void TxJoinServer::onDisconnect(Tx_Join::TxJoinException, Tx_Join::TxTcpSession::Ptr )
{

}

void TxJoinServer::blockNotify(int16_t  , int64_t  )
{
	/*(void*) _groupID ; 
    (void*) _blockNumber;*/
}
//  多线程处理 
static const int c_seqAbridgedLen = 8;
void TxJoinServer::onClientRequest(Tx_Join::TxTcpSession::Ptr session ,  Tx_Join::TxJoinException e, TxMessage::Ptr message)
{
     if (e.errorCode() == 0)
     {
        //std::cout<< " onMessage onClientRequest "<<  message->type() << std::endl;     
        switch (message->type())
        {
            case TX_TRAN_NO:
                onClientTran(session, message);
                break;
            case TX_PATCH_TRAN_NO:
                onClientPatchTran( session, message, 0); // same send tx to all node 
                break;
            case TX_RPC_PATCH_TRAN_NO:
                 onClientPatchTran( session, message,1); // only send one node 
                 break;   
            case  TX_GET_BLOCK_NO:
                onClientGetBlockNo(session, message);
                break;
            case  TX_SIM_TRAN_NO:
                onClientSimTran(session, message); // simple code transaction
                break;   
        }

     }
	 else
     {

     }
}

void TxJoinServer::onClientSimTran(Tx_Join::TxTcpSession::Ptr , TxMessage::Ptr message )
{
    u256 value = 0;
    u256 gasPrice = 0;
    u256 gas = 10000000;
    Address dest = Address(0x5002);

    dev::eth::Transaction::Ptr  tx = std::make_shared<dev::eth::Transaction>(value, gasPrice, gas, dest); 
    dev::bytes  tx_data(message->data(), message->data() + message->dataSize());
        
    bytesConstRef  tx_bcf(tx_data.data() ,tx_data.size());        

//static std::atomic<size_t> seq;
    uint64_t tcout = _txcount.fetch_add(1) + 1;

    if(tcout%10000==0)
    {
        TXJOIN_LOG(INFO) << "TxJoinServer  TX_SIM_TRAN_NO" << tcout << " msg seq "<< message->seq().substr(24, c_seqAbridgedLen);
    }               
    bytes data ;
    uint32_t userA1 ,userB1 ,money32;
    dev::eth::ContractABI abi;
    tx->simpledecode(tx_bcf,userA1, userB1,money32  );
    data = abi.abiIn("userTransfer(string,string,uint256)", to_string(userA1), to_string(userB1), u256(money32));
    auto ptr_data =  std::make_shared<bytes>(data ) ;
    tx->setData(ptr_data);

    //tx->setRpcTx(true); // set then this tx will sync other node ;           
    _ledger->txPool(message->groupID())->submit(tx);                 
}

void TxJoinServer::onClientGetBlockNo(Tx_Join::TxTcpSession::Ptr session ,    TxMessage::Ptr message ) 
{
    std::shared_ptr<dev::blockchain::BlockChainInterface>  blockChain  =  _ledger->blockChain( message->groupID());
    int64_t block_no  = blockChain->number();
    //if(_currentBlockNo != block_no)
    { 
        _currentBlockNo = block_no;    
        auto channelMessage = _server->messageFactory()->buildMessage();
        // only used for network statistic
        channelMessage->setGroupID(message->groupID());

        channelMessage->setType(TX_BLOCK_NO);
        channelMessage->setResult(0);
        channelMessage->setSeq(message->seq());
        int64_t  pendsize =  _ledger->txPool(message->groupID())->pendingSize();
        BLOCK_AND_PEND  bck_and_pend{block_no,pendsize};
        
        channelMessage->setData((const byte*)&bck_and_pend, sizeof(BLOCK_AND_PEND ));
        
        //std::cout<< " onMessage onClientRequest block no send  "<<  block_no  << std::endl;
        session->asyncSendMessage(channelMessage, std::function<void(Tx_Join::TxJoinException, TxMessage::Ptr)>() );

    } 
}
// send to fpga  and read from fpga
void TxJoinServer::onClientPatchFpgaTran(Tx_Join::TxTcpSession::Ptr  ,    TxMessage::Ptr message ,int rpcflag )
{
    char *  hash_array = new char[(64+1)*10000]; // 字符串
    char *  sig_r_array= new char[(64+1)*10000];
    char *  sig_s_array= new char[(64+1)*10000];
    char *  pubkey_array= new char[(128+1)*10000];
    int     tx_Num =10000; // test  max tx num is 2000;

    dev::bytes  tx_data(message->data(), message->data() + message->dataSize());                
    bytesConstRef  b_ref(tx_data.data() ,tx_data.size()); 

    std::shared_ptr<Transactions>  patchList2; 
    patchList2 = std::make_shared< std::vector<dev::eth::Transaction::Ptr>>(); 
                
    TxsParallelParser::decode(patchList2 ,b_ref ,hash_array,sig_r_array,sig_s_array ,tx_Num );  
    
    //fpga_api(hash_array,sig_r_array,sig_s_array,tx_Num, pubkey_array );

    for(int i= 0;i< tx_Num ;i++)
    {
        (*patchList2)[i]->setSender(pubkey_array+i*129 );
    }
    // set publib key from fpga card.  
    for (auto tx : *patchList2)
    {
        uint64_t tcout = _txcount.fetch_add(1) + 1;
        if(tcout%10000==0)
        {
            TXJOIN_LOG(INFO) << "TxJoinServer start receive" << tcout << " msg seq "<< message->seq().substr(24, c_seqAbridgedLen);
        }   

        if(rpcflag ==1)
        {    tx->setRpcTx(true); }//  set then this tx will sync other node ;   send all node ,no need set rpctx

        _ledger->txPool(message->groupID())->submit(tx);   //  first  way               
			                                 
    }
    delete 	 [] hash_array;
    delete 	 [] sig_r_array;
    delete 	 [] sig_s_array;  
    delete   [] pubkey_array;     
}

void TxJoinServer::onClientPatchTran(Tx_Join::TxTcpSession::Ptr  ,    TxMessage::Ptr message ,int rpcflag )
{
    dev::bytes  tx_data(message->data(), message->data() + message->dataSize());                
    bytesConstRef  b_ref(tx_data.data() ,tx_data.size()); 

    std::shared_ptr<Transactions>  patchList2; 
    patchList2 = std::make_shared< std::vector<dev::eth::Transaction::Ptr>>(); 
                
    TxsParallelParser::decode(patchList2 ,b_ref ,true  );  
    for (auto tx : *patchList2)
    {
        uint64_t tcout = _txcount.fetch_add(1) + 1;
        if(tcout%10000==0)
        {
            TXJOIN_LOG(INFO) << "TxJoinServer start receive" << tcout << " msg seq "<< message->seq().substr(24, c_seqAbridgedLen);
        }   
        if(rpcflag ==1)
        {    tx->setRpcTx(true); }//  set then this tx will sync other node ;   send all node ,no need set rpctx

        _ledger->txPool(message->groupID())->submit(tx);   //  first  way               
        /*         
        auto txpool = _ledger->txPool(message->groupID());        // second way
        /// check the verify result(nonce && signature check)
        ImportResult  importResult;
        ImportResult verify_ret = txpool->verify(tx);
        if (verify_ret == ImportResult::Success)
        {   
            
            importResult = txpool->import(tx);
            if (dev::eth::ImportResult::Success == importResult)
            {

            }
        }*/				 
                                
    }	            
}

void TxJoinServer::onClientTran(Tx_Join::TxTcpSession::Ptr  ,    TxMessage::Ptr message)
{
    dev::eth::Transaction::Ptr  tx = std::make_shared<dev::eth::Transaction>(); 
    dev::bytes  tx_data(message->data(), message->data() + message->dataSize());
        
    bytesConstRef  bcf(tx_data.data() ,tx_data.size());        

//static std::atomic<size_t> seq;
    uint64_t tcout = _txcount.fetch_add(1) + 1;

    if(tcout%10000==0)
    {
        TXJOIN_LOG(INFO) << "TxJoinServer start receive" << tcout << " msg seq "<< message->seq().substr(24, c_seqAbridgedLen);
    }   
    
    tx->decode(bcf ); 
    //tx->setRpcTx(true); // set then this tx will sync other node ;           
    _ledger->txPool(message->groupID())->submit(tx);
}

void TxJoinServer::setListenAddr(const std::string& listenAddr)
{
    _listenAddr = listenAddr;
}

void TxJoinServer::setListenPort(int listenPort)
{
    _listenPort = listenPort;
}





