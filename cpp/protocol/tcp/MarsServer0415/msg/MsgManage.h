#ifndef __EXPLAINPACK_H__
#define __EXPLAINPACK_H__
#include "../public/PublicFun.h"
#include"./mysql/EncapMysql.h"
#include <pthread.h>
#include "MsgInfo.h"
#include"./protobuf/chat.pb.h"

class CMsgManage
{

public:
    CMsgManage();
    ~CMsgManage();
    bool Init();
    bool Deal(const USERDATAINFO& usrData );
    bool IsvalidMsg(int fd, uint32_t cmd);

    //消息队列
    void MsgDequeSendPush(USERDATAINFO& msgData);
    void MsgDequeSendPopFront();
    void PrintData(char* pData, int nLen);

private:
    bool DealKeepLive(const USERDATAINFO& usrInfo);

    //上线
    bool DealLogin(const USERDATAINFO& usrInfo);

    //同一帐号只允许登陆一次
    bool SendDeviceLogout(int fd);

    //下线
     bool DealLogout(const USERDATAINFO& usrInfo);

    //加好友：clientA---->server----->clientB
    bool DealAddContactReqByClientA(const USERDATAINFO& usrInfo);
    bool TransformServerToB(int fd, const char* strCoversationID, uint32_t optionType, int32_t userIda);
    bool SendMsgToB( int fd, const char* strCoversationID, uint32_t optiontype, int32_t userIda);
    bool StoreAddcontactToDB(const  MarsMsgChat::ContactAdd&  contact);
    bool GetConversationIDFromContactDB(uint32_t userIdA, uint32_t userIdB, string& strCoversationID);

    //添加非在线用户为好友，存入数据库, ClientA
    bool StroreAddContactOfflineToDBByClientA(const  MarsMsgChat::ContactAdd& usrInfo, const char* strCoversationID);

    //加好友：clientB---->server----->clientA
    bool DealAddContactRspByClientB(const USERDATAINFO& usrInfo);
    bool TransformServerToA( int fd, const char* strCoversationID,int32_t optionType,  int32_t userIdb);
   bool SendMsgToA( int fd, const char* strCoversationID, int32_t optionType,  int32_t userIdb);
    bool StoreResultToDB( const  MarsMsgChat::ContactAdd&  contact);

    //添加好友结果回执，存入数据库 ClientB
    bool StroreAddContactToDBByClientB(const  MarsMsgChat::ContactAdd& usrInfo);


     //私聊
    bool DealChatPrivate(const USERDATAINFO& usrInfo);
    bool TransmitMsgToB(const MarsMsgChat::SendMsgReq& msg,const USERDATAINFO& Data, const uint32_t iSocket );
    bool AckToA(const MarsMsgChat::SendMsgReq& msgReq, const USERDATAINFO& Data,  const int32_t errorCode);
    bool StoreConversationID(const MarsMsgChat::SendMsgReq& msg);
    bool StoreMsgToDB(const MarsMsgChat::SendMsgReq& msg);
    bool GetConversationID(const MarsMsgChat::SendMsgReq& msg, string& strCoversationID );

    //群聊
    bool DealChatGroup(const USERDATAINFO& usrInfo);
    bool  GetGroupMember(const MarsMsgChat::SendMsgReq& msg);
    bool SendBoardcastMsg(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data);
    bool SendAckToClientB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data,  const int32_t errorCode);

    /****************************BEGIN:   离线消息处理 ***************************/
    //离线消息处理(此处不处理群聊消息记录)
    bool DealOfflineMsg(int32_t userId);
    bool DealOfflineMsgDetail(int32_t userId,enOfflineDealMethod nType);

    bool GetOfflienMsg(int32_t userId);
    bool DealOfflineData(const PSTOFFLINEMSG pMsg, const uint32_t userId);
    bool IsNeedDealMsg(const PSTOFFLINEMSG pMsg, const uint32_t userId, enOfflineDealMethod nType);
    bool UpdateOfflinePushInfo(uint32_t userId, uint32_t groupId);

    //离线好友请求与应答
    bool DealOfflineAddContactReq(const PSTOFFLINEMSG pMsg, int fd);
    bool DealOfflineAddContactRsp(const PSTOFFLINEMSG pMsg, int fd);

    //离线私聊消息
    //bool DealOfflinePrivateChat(const PSTOFFLINEMSG pMsg, int fd);
     bool SendOfflinePrivateChat(const PSTOFFLINEMSG pMsg, int fd);

     //离线群聊消息处理(此处处理群聊离线消息记录)
     bool GetGroupChatOfflienMsg(int32_t userId);
     bool DealOfflineGroupChat(const PSTOFFLINEMSG pMsg,  const int  fd);


     //离线回执消息处理
     bool DealOfflineByClientB(const USERDATAINFO& usrInfo);
     bool RemoveOfflineMsgInDB(const MarsMsgChat::MsgOffline&  msg);

/****************************END:   离线消息处理 ***************************/

     //公用函数
     bool GetuserInfoFromDB(int32_t userId,  EncapMysql::CEncapMysql* pMysql);
     bool GetGroupInfoFromDB(int32_t userId,  EncapMysql::CEncapMysql* pMysql);

     bool ExecuteMysql(const char* szSql);
     bool GetuserInfo(const PSTOFFLINEMSG pMsg);
    bool GetGroupInfo(const PSTOFFLINEMSG pMsg);



public:
    CPublicFun* m_pFun;
    EncapMysql::CEncapMysql* m_mySqlEncap;

     EncapMysql::CEncapMysql* m_mySqlEncapOffline;
    USERDATAINFO m_usrSend;
    std::deque<USERDATAINFO> m_msgDuqueSend;
    pthread_mutex_t m_mtxMsgDuqueSend;


private:
    char  m_szSql[1024];
    uint32_t  m_uCmdId;
    uint32_t m_userIdLogin;
    set<uint32_t> m_setGroupOnLine;
    vector<string> m_vcOfflineMsg;
    PSTOFFLINEMSG m_pOfflineMsg;

    char m_szHead[264];
    char m_szName[264];








};















#endif // __EXPLAINPACK_H__
