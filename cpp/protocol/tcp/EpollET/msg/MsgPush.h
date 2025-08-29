#ifndef CPUSHRSP
#define CPUSHRSP

#include "IMsgBase.h"



class CMsgPush : public IMsgBase
{
public:
    CMsgPush();
    ~CMsgPush();
    void Deal(const USERDATAINFO& Data);
    int32_t PushOffLineMsg(int32_t usrID, int nSocket);

private:
    bool  SendMsgToB(int nSocket);


    bool ExistContactAddMsg(uint32_t userid);
    bool ExistChatPrivateMsg();
    bool ExistChatGroupMsg();

    bool DealContactAddMsg();
    bool DealChatPrivateMsg();
    bool DealChatGroupMsg();
};

#endif // CPUSHRSP

