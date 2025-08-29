#include "PublicFun.h"
#include<arpa/inet.h>
#include<ctime>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/time.h>
#include <unistd.h>

using namespace std;

#define SERVER_CONFIG_FILE_PATH  ("./config.conf")

CPublicFun::CPublicFun()
{

}

CPublicFun::~CPublicFun()
{

}

CPublicFun* CPublicFun::GetInstance()
{
	static CPublicFun* pFun = NULL;
	if(pFun == NULL)
	{
		pFun = new CPublicFun();
	}

    return pFun;
}

bool CPublicFun::Init()
{
    m_ServerConfig.SetFilePath(SERVER_CONFIG_FILE_PATH);
    m_ServerConfig.Parse();

    int nRet = m_sqlCom.Init(m_ServerConfig.m_mapConfig["USER_DB_HOST"], atoi(m_ServerConfig.m_mapConfig["USER_DB_PORT"].c_str()),
            m_ServerConfig.m_mapConfig["USER_DB_USERNAME"], m_ServerConfig.m_mapConfig["USER_DB_PASSWORD"],m_ServerConfig.m_mapConfig["USER_DB_NAME"] , 3);
    if(nRet != 0)
    {
        return false;
    }
    return true;
}

void CPublicFun::AwakenWriteEvent(int nSocket)
{
    struct epoll_event ev;
    ev.events = EPOLLOUT |EPOLLET;
    ev.data.fd = nSocket;
    if(epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, nSocket, &ev))
    {
        printf("CPublicFun::AwakenWriteEvent() epoll_ctl failed! nSocket:%d, close and delete it! \n", nSocket);
        AwakenDelEvent(nSocket);

    }
    else
    {
          //printf("CPublicFun::AwakenWriteEvent() epoll_ctl success! \n", nSocket);
    }
}

void CPublicFun::AwakenReadEvent(int nSocket)
{


}

void CPublicFun::AwakenDelEvent(int nSocket)
{
    close(nSocket);
    struct epoll_event event_del;
    event_del.data.fd  = nSocket;
    event_del.events = 0;
    epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, nSocket,  &event_del);
}

int64_t CPublicFun::GetSystemTime()
{
    struct timeval tv;
      gettimeofday(&tv,NULL);
      return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}





 void CPublicFun::PrintLog(const char* pData)
 {

     time_t ti = time(0);
     tm t = *localtime(&ti);

     char buf[128];
     memset(buf, 0,sizeof(buf));

      sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d",t.tm_year + 1900,t.tm_mon + 1,t.tm_mday, \
              t.tm_hour,t.tm_min, t.tm_sec);

      string strTime = buf;
      strTime +=" ";
      strTime += pData;

      cout << strTime << endl;
 }
