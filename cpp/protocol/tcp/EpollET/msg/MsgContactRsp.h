#ifndef MSGCONTACTRSP
#define MSGCONTACTRSP
#include "IMsgBase.h"
#include"../protobuf/ContactAdd.pb.h"


class CMsgContactRsp : public  IMsgBase
{
public:
    CMsgContactRsp();
    ~CMsgContactRsp();

public:
    void Deal(const USERDATAINFO& Data);

private:
    bool TransformServerToA( const MarsContactAdd::ContactAdd& contact, int iSocket);
    bool SendMsgToA( const MarsContactAdd::ContactAdd&, int iSocket);

};




#endif // MSGCONTACTRSP

