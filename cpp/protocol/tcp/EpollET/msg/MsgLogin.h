#ifndef CMSGLOGIN
#define CMSGLOGIN
#include "IMsgBase.h"




class CMsgLogin : public  IMsgBase
{
public:
    CMsgLogin();
    ~CMsgLogin();

public:
    void Deal(const USERDATAINFO& Data);


};






















#endif // CMSGLOGIN

