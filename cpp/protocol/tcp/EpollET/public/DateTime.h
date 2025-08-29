#ifndef DATETIME
#define DATETIME
#include "PublicFun.h"



typedef struct st_myTime
{
    uint32_t Year;
    unsigned char  Month;
    unsigned char Day;
    unsigned char  Hour;
    unsigned char Min;
    unsigned char Second;
}STDATETIME,*PSTDATETIME;


class CDateTime
{
public:
    CDateTime() {};
    ~CDateTime() {};
    CDateTime(const STDATETIME& stTime);


private:
    STDATETIME m_stDateTime;

};


#endif // DATETIME

