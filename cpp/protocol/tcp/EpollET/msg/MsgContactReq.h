#ifndef MSGCONTACTADD
#define MSGCONTACTADD

#include "IMsgBase.h"
#include"../protobuf/ContactAdd.pb.h"











class CMsgContactAdd : public  IMsgBase
{
public:
    CMsgContactAdd();
    ~CMsgContactAdd();

public:
    void Deal(const USERDATAINFO& Data);

private:

     bool TransformServerToB(const  MarsContactAdd::ContactAdd&  contact, int iSocket);
     bool SendMsgToB( const  MarsContactAdd::ContactAdd&  contact, int iSocket);
     bool StoreContactReqToDB(const  MarsContactAdd::ContactAdd&  contact);


};








#endif // MSGCONTACTADD

