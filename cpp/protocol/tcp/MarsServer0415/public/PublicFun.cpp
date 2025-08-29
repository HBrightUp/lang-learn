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
    pthread_mutex_destroy(&m_mtxOffline);
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
     pthread_mutex_init(&m_mtxOffline, NULL);




    m_ServerConfig.SetFilePath(SERVER_CONFIG_FILE_PATH);
    m_ServerConfig.Parse();

    int num = 2;
    int nRet = m_sqlCom.Init(m_ServerConfig.m_mapConfig["USER_DB_HOST"], atoi(m_ServerConfig.m_mapConfig["USER_DB_PORT"].c_str()),
            m_ServerConfig.m_mapConfig["USER_DB_USERNAME"], m_ServerConfig.m_mapConfig["USER_DB_PASSWORD"],m_ServerConfig.m_mapConfig["USER_DB_NAME"] , num);
    if(nRet != 0)
    {
        return false;
    }

    printf("[ mysql ] connnect success. Nummber:%d,user:%s,ip:%s\n",
           num, m_ServerConfig.m_mapConfig["USER_DB_USERNAME"].data(),m_ServerConfig.m_mapConfig["USER_DB_HOST"].data());

    return true;
}

bool CPublicFun::GetAddressBySocket(int fd, sockaddr_in& addr)
{
    memset(&addr, 0, sizeof(addr));
    uint nlen = sizeof(addr);

    //根据套接字获取地址信息
    if(::getpeername(fd, (sockaddr*)&addr, &nlen) != 0)
    {
        return false;
    }

    return true;
}


int CPublicFun::EpollCtlFd( int epollfd, int fd, EnFdCtl ctl, uint32_t events)
  {
    if(fd <=0)
        return  -1;
      struct epoll_event event;
      event.data.fd = fd;
      int iRet = 0;

      if(ctl == ENU_FD_ADD)
      {
          SetNonblock( fd );
        //  event.events = EPOLLIN |  EPOLLONESHOT ;
           event.events = EPOLLIN ;
           iRet =epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );

      }
      else if(ctl == ENU_FD_MOD)
      {
          if(events & EPOLLIN)
          {
               printf("epoll_ctl()   fd:%d, READ \n", fd);
          }
          else if(events & EPOLLOUT)
          {
               printf("epoll_ctl()   fd:%d,   WRITE\n", fd );
          }
          else
          {

          }




         // event.events =  events | EPOLLONESHOT;
           event.events =  events ;
          iRet = epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
      }
      else if(ctl == ENU_FD_DEL)
      {
          event.events = 0;
         iRet= epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
      }
      else
      {

      }



      if(iRet != 0)
      {
          printf("epoll_ctl() failed. fd:%d, ctl:%d,  errno:%d\n", fd, ctl, errno);
      }

      return iRet;
  }


void CPublicFun::GetSystemTime(string& str)
{
    struct timeval tv;
      gettimeofday(&tv,NULL);
      uint64_t data =  tv.tv_sec * 1000 + tv.tv_usec / 1000;

      stringstream stream;
      stream << data;

      str = stream.str();
      return ;
}

int CPublicFun::SetNonblock(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);

    return old_option;
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

 void CPublicFun::StopConnect(int fd)
 {
     sockaddr_in addr;
     CPublicFun::GetInstance()->GetAddressBySocket(fd, addr );
     printf("[ DISCONNECT ] IP:%s, Port:%d, fd:%d\n",  ::inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),  fd);

     CPublicFun::GetInstance()->EpollCtlFd(CPublicFun::GetInstance()->m_nEpollFd, fd, ENU_FD_DEL);

      close(fd);

     CUsrManage::GetInstance()->DelUsrBySocket(fd);

     return ;
 }
