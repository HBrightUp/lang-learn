#include "EpollServer.h"
#include<errno.h>
#include"../public/PublicFun.h"
#include "../usr/UserManage.h"
#include"MsgDispose.h"


#define MAX_SOCKET_BACKLOG 20

using namespace std;

CEpollServer::CEpollServer()
{
    m_pFun = CPublicFun::GetInstance();
}

CEpollServer::~CEpollServer()
{
    close(m_nListenFd);
}

CEpollServer* CEpollServer::GetInstance()
{
    static CEpollServer* pServer = 0;
    if(!pServer)
    {
        pServer = new CEpollServer();
    }

    return pServer;
}




bool CEpollServer::InitServer()
{
    CPublicFun::GetInstance()->Init();

    m_pMsgPose = new CMsgDispose[MAX_SOCKFD_CONNECT];
    if(!m_pMsgPose)
    {
        return false;
    }

    m_pThreadPool = new threadpool<CMsgDispose>(10);
    if(!m_pThreadPool)
    {
        return false;
    }

    m_nEpollFd = epoll_create(MAX_SOCKFD_CONNECT);
    m_pFun->m_nEpollFd = m_nEpollFd;

    if((m_nListenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return false;
    }

    m_pFun->SetNonblock(m_nListenFd);

    string strIP = CPublicFun::GetInstance()->m_ServerConfig.m_mapConfig["HOST"];
    string strPort = CPublicFun::GetInstance()->m_ServerConfig.m_mapConfig["PORT"];

    sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(atoi(strPort.data()));
    listen_addr.sin_addr.s_addr = inet_addr(strIP.data());

   int nReuseaddr = 1;
    int nRet = setsockopt(m_nListenFd, SOL_SOCKET, SO_REUSEADDR, &nReuseaddr, sizeof(nReuseaddr));
    if(nRet == -1)
    {
        printf("[ ERROR ]  set    SO_REUSEADDR  failed!\n ");

    }


      if( setsockopt( m_nListenFd , SOL_SOCKET, SO_REUSEPORT,( char *)&nReuseaddr, sizeof( nReuseaddr ) ) < 0 )
      {
          printf("[ ERROR ]  set    SO_REUSEPORT  failed!\n ");
      }





    if(bind(m_nListenFd, (sockaddr*)&listen_addr, sizeof(listen_addr)) != 0)
    {
        printf("[ ERROR ] bind port failed,IP:%s,Port:%s, errno:%d\n", strIP.c_str(), strPort.c_str(), errno);
       close(m_nListenFd);
        return false;
    }

    if(listen(m_nListenFd, MAX_SOCKET_BACKLOG) < 0)
    {
        close(m_nListenFd);
        return false;
    }
    else
    {
              printf("[ LISTEN ]  IP:%s,Port:%s\n", strIP.c_str(), strPort.c_str());
    }

    m_pFun->EpollCtlFd(m_nEpollFd, m_nListenFd, ENU_FD_ADD);

    return true;
}


void CEpollServer::Run()
{
    struct epoll_event events[MAX_SOCKFD_CONNECT];
    sockaddr_in remote_addr;
    int addrLen = sizeof(remote_addr);


    int fd,nRecv,nSend,nSize,nfds,i;
    fd = nRecv = nSend = nSize = nfds = i = 0;


    while(true)
    {
        nfds = epoll_wait(m_nEpollFd, events, MAX_SOCKFD_CONNECT, -1);

        for(i = 0; i < nfds; ++i)
        {
            fd = events[i].data.fd;
            if(fd == m_nListenFd)
            {
                int clientFd = accept(m_nListenFd,(sockaddr*)&remote_addr, (socklen_t*)&addrLen);
                if(clientFd < 0)
                {
                    printf("[ ACCEPT ] failed. fd:%d, errno:%d\n", clientFd, errno);
                    continue;
                }
                if(CUsrManage::GetInstance()->m_setUsrOnline.size() >= MAX_SOCKFD_CONNECT - 1)
                {
                     printf("[ ACCEPT ] . fd:%d\n", clientFd);
                     continue;
                }
                printf("[ ACCEPT ] socket:%d, IP:%s,Port:%d\n",   clientFd,inet_ntoa(remote_addr.sin_addr),remote_addr.sin_port );
                m_pFun->EpollCtlFd(m_nEpollFd, clientFd, ENU_FD_ADD);
                m_pMsgPose[clientFd].Init(clientFd, remote_addr);
            }
            else if(events[i].events & EPOLLIN)
            {
                if(m_pMsgPose[fd].Read())
                {
    #if 0
                    static uint32_t readFd = 0;

                    if(readFd  == fd)
                    {
                        while(true)
                        {
                            usleep(20000);
                        }
                    }
#endif

                    m_pThreadPool->append(m_pMsgPose + fd);
                }
                else
                {
                     printf("[ DISCONNECT ] Read. fd:%d, errno:%d\n", fd, errno);
                     m_pFun->StopConnect(fd);
                }
             }
             else if(events[i].events & EPOLLOUT)
             {
                if(!m_pMsgPose[fd].Write())
                {
                    printf("[ DISCONNECT ] Write. fd:%d, errno:%d\n", fd, errno);
                    //m_pMsgPose[fd].StopConnect();
                    m_pFun->StopConnect(fd);
                }
                else
                {
                    m_pFun->EpollCtlFd(m_nEpollFd, fd, ENU_FD_MOD, EPOLLIN);
                }
            }
             else if(events[i].events & (EPOLLERR | EPOLLHUP | EPOLLERR))
            {
                printf("[ DISCONNECT ] EPOLLER. fd:%d, errno:%d\n", fd, errno);
                //m_pMsgPose[fd].StopConnect();
                m_pFun->StopConnect(fd);
            }
            else
            {
                     printf("events:%d, socket:%d\n",events[i].events,events[i].data.fd);
            }
        }
    }
}







