#ifndef CSENDMSGREQ
#define CSENDMSGREQ

#include "IMsgBase.h"
#include "../public/PublicFun.h"
#include"../protobuf/chat.pb.h"





class CMsgChatPrivate : public IMsgBase
{
public:
    CMsgChatPrivate();
    ~CMsgChatPrivate();
    void Deal(const USERDATAINFO& Data);

private:

    //将会话信息存入数据库tb_conversation
    bool StoreConversationID(const MarsMsgChat::SendMsgReq& msg);
    bool StoreMsgToDB(const MarsMsgChat::SendMsgReq& msg);
    bool TransmitMsgToB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data, const uint32_t iSocket);

    bool AckToA(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data, const int32_t errorCode);

    //从数据库tb_contacts中取出conversationID返回给客户端
    bool GetConversationID(const MarsMsgChat::SendMsgReq& msg,  string& strConversationID);

    //判断B是否在线,在线时socket是有效的
    int GetRecvUsrSocket(const string& usrTo);



   // bool ExecuteMysql(const char* szSql);
    bool SelectMysql(const char* szSql);


private:
    int64_t m_uUniqueMsgID;

};




























#endif // CSENDMSGREQ

