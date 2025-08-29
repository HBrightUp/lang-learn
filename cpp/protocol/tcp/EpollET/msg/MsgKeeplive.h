#include "IMsgBase.h"



class CMsgKeeplive : public IMsgBase
{
public:
    CMsgKeeplive();
    ~CMsgKeeplive();
    void Deal(const USERDATAINFO& Data);


};
