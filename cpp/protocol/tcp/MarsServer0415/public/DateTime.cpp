#include"PublicFun.h"
#include "DateTime.h"


string CDateTime::GetStdTime()
{
    string strTime;

#if 0
    m_stDateTime.Year = 1900 + p->tm_year;
    m_stDateTime.Month = 1 + p->tm_mon;
    m_stDateTime.Day = p->tm_mday;
    m_stDateTime.Hour = p->tm_hour;
    m_stDateTime.Min = p->tm_min;
    m_stDateTime.Second = p->tm_sec;

    char buf[64];
    memset(buf, 0, sizeof(buf));
    snprintf(buf,sizeof(buf), "%d-%2d-%2d %2d:%2d:%2d", stTime.Year, stTime.Month,stTime.Day, stTime.Hour, stTime.Min, stTime.Second);

#endif
    return strTime;
}
