#include "./EpollServer.h"
#include<errno.h>
#include"../public/PublicFun.h"
#include "../usr/UserManage.h"


#define MAX_SOCKET_BACKLOG 20

using namespace std;

CEpollServer::CEpollServer()
{
    m_pManage = CMsgManage::GetInstance();
    m_pFun = CPublicFun::GetInstance();
}

CEpollServer::~CEpollServer()
{
}

  int CEpollServer::SetNonblock(int fd)
  {
      int old_option = fcntl( fd, F_GETFL );
      int new_option = old_option | O_NONBLOCK;
      fcntl( fd, F_SETFL, new_option );

      return old_option;
  }

  int CEpollServer::EpollFdOption( int epollfd, int fd, EnFdOption fdOption, uint32_t events)
  {
      struct epoll_event event;
      event.data.fd = fd;
      int iRet = 0;

      if(fdOption == ENU_FD_ADD)
      {
          SetNonblock( fd );
          event.events = EPOLLIN | EPOLLET;
           iRet =epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );

      }
      else if(fdOption == ENU_FD_MODIFY)
      {
          event.events =  events;
          iRet = epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
      }
      else if(fdOption == ENU_FD_DEL)
      {
          event.events = 0;
         iRet= epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
      }
      else
      {

      }

      if(iRet != 0)
      {
          printf("epoll_ctl() failed. fd:%d, fdOption:%d,  errno:%d\n", fd, fdOption, errno);
      }


      return iRet;
  }

bool CEpollServer::InitServer()
{
    InitSignal();

    m_nEpollFd = epoll_create(MAX_SOCKFD_CONNECT);
    m_pFun->m_nEpollFd = m_nEpollFd;

    int nPort = atoi(m_pFun->m_ServerConfig.m_mapConfig["PORT"].data());
    string strIP = m_pFun->m_ServerConfig.m_mapConfig["HOST"];

    m_nSock = socket(AF_INET, SOCK_STREAM, 0);

    SetNonblock(m_nSock);


    sockaddr_in listen_addr;
    bzero(&listen_addr, sizeof(listen_addr));

    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(nPort);
    listen_addr.sin_addr.s_addr = inet_addr(strIP.c_str());

    int nReuse_on = 1;
    setsockopt(m_nSock, SOL_SOCKET, SO_REUSEADDR, &nReuse_on, sizeof(nReuse_on));

    if(bind(m_nSock, (sockaddr*)&listen_addr, sizeof(listen_addr)) != 0)
    {
        printf("bind error!errno:%d\n",errno);
        return false;
    }

    if(listen(m_nSock, MAX_SOCKET_BACKLOG) < 0)
    {
        printf("listen error!\n");
        return false;
    }
    else
    {
              printf("[ LISTEN ] Start listen IP:%s,Port:%d\n", strIP.data(), nPort);
    }

    if(pthread_create(&m_thListenThreadId, NULL, ListenThread, this))
    {
        printf("Create Listen thread failed!\n");
    }

    return true;
}
int CEpollServer::InitSignal()
{
    signal(SIGPIPE, SIG_IGN);
//signal(SIGPIPE, pipesig_handler);
    return 0;
}
bool CEpollServer::GetAddressBySocket(int iSocket, sockaddr_in& addr)
{
    memset(&addr, 0, sizeof(addr));
    uint nlen = sizeof(addr);

    //根据套接字获取地址信息
    if(::getpeername(iSocket, (sockaddr*)&addr, &nlen) != 0)
    {
        return false;
    }

    return true;
}

void* CEpollServer::ListenThread(void* lpParam)
{
    CEpollServer* pThis = (CEpollServer*)lpParam;

    sockaddr_in remote_addr;
    int len = sizeof(remote_addr);
    int client_socket = 0;

    while(true)
    {
        client_socket = accept(pThis->m_nSock,(struct sockaddr*)&remote_addr, (socklen_t*)&len);
        if(client_socket > 0)
        {
            printf("[ ACCEPT ]  IP:%s, Port:%d, Socket:%d\n",inet_ntoa(remote_addr.sin_addr),remote_addr.sin_port, client_socket );

            pThis->EpollFdOption(pThis->m_nEpollFd, client_socket, ENU_FD_ADD);
        }

        if (client_socket == -1)
        {
            if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
            {
                //accept
            }
        }
        usleep(300);

    }
}


void CEpollServer::Run()
{

    USERDATAINFO msgRecvData;
    USERDATAINFO msgSendData;
    CMsgManage* pMsgMan = CMsgManage::GetInstance();
    struct epoll_event events[MAX_SOCKFD_CONNECT];

     int recv_size,send_size,client_socket,nfds,nMsgDequeSize,i,nPos,nLen;
     recv_size = send_size =client_socket =nfds = nMsgDequeSize = i =nPos=nLen= 0;

    while(true)
    {
        nfds = epoll_wait(m_nEpollFd, events, MAX_SOCKFD_CONNECT, -1);

        for(i = 0; i < nfds; ++i)
        {
            client_socket = events[i].data.fd;

            if(events[i].events & EPOLLIN)
            {
                  nPos = 0;
                  while ((recv_size = read (client_socket,  &msgRecvData.usrPackage + nPos, MARS_MSG_HEAR_LEN + MARS_USRDATA_MAX  - 1)) > 0)
                  {
                      nPos += recv_size;
                  }
                  if (recv_size == -1 && errno != EAGAIN)
                  {
                      perror ("read error");
                        StopConnect(client_socket);
                  }

                  printf("RecvData:%d,socket:%d\n", nPos, client_socket);
                  msgRecvData.iSocket = client_socket;
                  MsgRecvDuequPushBack(msgRecvData);

             }
             else if(events[i].events & EPOLLOUT)
             {
                nMsgDequeSize = pMsgMan->m_msgDuqueSend.size();
                for(i = 0; i < nMsgDequeSize; ++i)
                {
                    msgSendData = pMsgMan->m_msgDuqueSend.front();
                    nPos = 0;
                    nLen = msgSendData.nMsgLen;
                    printf("[ SEND]  nLen:%d\n", nLen);

                    while (nLen > 0)
                    {
                        nPos = write (client_socket, &msgSendData.usrPackage + msgSendData.nMsgLen - nLen, nLen);
                        printf("[ SEND]  Datasize:%d\n", nPos);
                        if (nPos <= nLen)
                        {
                            if (nPos ==  -1 && errno != EAGAIN)
                            {
                                printf("[ SEND ] error. socket:%d\n", client_socket);
                                StopConnect(client_socket);
                            }

                             pMsgMan->MsgDequeSendPopFront();
                             EpollFdOption(m_nEpollFd, client_socket, ENU_FD_MODIFY, EPOLLET | EPOLLIN);
                            break;
                        }
                        nLen  -= nPos;
                    }

                }
            }
            else if(events[i].events & EPOLLWRNORM)
            {
                 printf("ERROR:  other epoll event:%d\n", events[i].events );
                EpollFdOption(m_nEpollFd, client_socket, ENU_FD_MODIFY, EPOLLET | EPOLLIN);
            }
            else
            {
                printf("ERROR:  other epoll event:%d, errno:%d\n", events[i].events, errno);
                //StopConnect(client_socket);
            }
        }

        usleep(20);
    }
}

void CEpollServer::StopConnect(int iSocket)
{
    if(iSocket  <=  0)
        return ;

    sockaddr_in addr;
    GetAddressBySocket(iSocket, addr );

    printf("[ DISCONNECT ] IP:%s, Port:%d, socket:%d\n",  ::inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),  iSocket);

    EpollFdOption(m_nEpollFd, iSocket, ENU_FD_DEL);
    close(iSocket);

    CUsrManage::GetInstance()->DelUsrBySocket(iSocket);
}

void CEpollServer::MsgRecvDuequPushBack(USERDATAINFO& msgInfo)
{
     m_pManage->MsgDequeRecvPush(msgInfo);
}

void CEpollServer::PrintData(char* pData, int nLen)
{
	for(int i = 0; i < nLen; ++i)
	{
		if( ('0' <= pData[i] && pData[i] <='9') || ('a' <= pData[i] && pData[i] <='z')
|| ('A' <= pData[i] && pData[i] <='Z'))
		{
			printf("%c", pData[i]);
		}
		else
		{
			printf(" %d ", pData[i]);
		}
	}
	cout << endl;
}


