#include <iostream>
#include<stdio.h>
#include <sys/time.h>
#include<sstream>
#include<memory.h>

using namespace std;

typedef struct st_myTime
{
    uint32_t Year;
    unsigned char  Month;
    unsigned char Day;
    unsigned char  Hour;
    unsigned char Min;
    unsigned char Second;
}STDATETIME,*PSTDATETIME;

int main()
{
#if 0
    struct timeval tv;
    gettimeofday(&tv,NULL);

    uint64_t data =   tv.tv_sec * 1000 + tv.tv_usec / 1000;

    cout <<  sizeof(data) << "  " << data << endl;


    int32_t  b = 152000;

    stringstream stream;
    stream << b;

    string str = stream.str();

    int32_t r = 1000;

    stream.str("");
    stream << r;
    str = stream.str();


    cout <<str.data()<< endl;

#endif
    string strTime;
    STDATETIME stTime;

      time_t timep;
      struct tm *p;
      time(&timep);
      p = localtime(&timep); //获取UTC时间

      stTime.Year = 1900 + p->tm_year;
      stTime.Month =1 + p->tm_mon;
      stTime.Day = p->tm_mday;
      stTime.Hour = p->tm_hour;
      stTime.Min = p->tm_min;
      stTime.Second = p->tm_sec;


      char buf[8];
      memset(buf, 0 , sizeof(buf));
      snprintf(buf, sizeof(buf), "%d", stTime.Year);
      strTime = buf;

      memset(buf, 0 , sizeof(buf));
      if(stTime.Month < 10)
      {
           snprintf(buf, sizeof(buf), "-0%d", stTime.Month);
      }
      else
      {
           snprintf(buf, sizeof(buf), "-%d", stTime.Month);
      }
      strTime += buf;

      memset(buf, 0 , sizeof(buf));
      if(stTime.Day < 10)
      {
           snprintf(buf, sizeof(buf), "-0%d ", stTime.Day);
      }
      else
      {
           snprintf(buf, sizeof(buf), "-%d ", stTime.Day);
      }
      strTime += buf;

      memset(buf, 0 , sizeof(buf));
      if(stTime.Hour < 10)
      {
           snprintf(buf, sizeof(buf), "0%d", stTime.Hour);
      }
      else
      {
           snprintf(buf, sizeof(buf), "%d", stTime.Hour);
      }
      strTime += buf;

      memset(buf, 0 , sizeof(buf));
      if(stTime.Min < 10)
      {
           snprintf(buf, sizeof(buf), ":0%d", stTime.Min);
      }
      else
      {
           snprintf(buf, sizeof(buf), ":%d", stTime.Min);
      }
      strTime += buf;

      memset(buf, 0 , sizeof(buf));
      if(stTime.Second < 10)
      {
           snprintf(buf, sizeof(buf), ":0%d", stTime.Second);
      }
      else
      {
           snprintf(buf, sizeof(buf), ":%d", stTime.Second);
      }
      strTime += buf;


      cout << strTime.c_str() << endl;
    return 0;
}

