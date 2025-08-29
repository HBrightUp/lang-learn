#ifndef MSGCHATGROUP
#define MSGCHATGROUP

#include "IMsgBase.h"
#include "../public/PublicFun.h"
#include"../protobuf/chat.pb.h"




class CMsgChatGroup: public IMsgBase
{
public:
    CMsgChatGroup();
    ~CMsgChatGroup();
    void Deal(const USERDATAINFO& Data);

private:
    bool GetGroupMember(const MarsMsgChat::SendMsgReq& msg);

    bool SendBoardcastMsg(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data);

    bool SendAckToClientB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data,  const int32_t errorCode);


private:
    set<uint32_t> m_setGroupOnLine;

};

























#endif // MSGCHATGROUP

