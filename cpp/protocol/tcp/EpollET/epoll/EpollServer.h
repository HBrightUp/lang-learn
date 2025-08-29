#ifndef _EPOLLSERVER_H_
#define _EPOLLSERVER_H_

#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<pthread.h>
#include<string.h>
#include <unistd.h>
#include "../public/PublicFun.h"
#include"../msg/MsgManage.h"


#define MAX_SOCKFD_CONNECT 65535

enum EnFdOption
{
    ENU_FD_ADD = 1,
    ENU_FD_MODIFY,
    ENU_FD_DEL,
};


class CEpollServer
{
public:
    CEpollServer();
    ~CEpollServer();

    bool InitServer();
    void Listen();
    static void* ListenThread(void* lpParam);
    void Run();
    void MsgRecvDuequPushBack(USERDATAINFO& msgInfo);

	void PrintData(char* pData, int nLen);

private:
    void StopConnect(int iSocket);
    int SetNonblock(int fd);
     int  EpollFdOption( int epollfd, int fd, EnFdOption fdOption, uint32_t events = 0);
     bool GetAddressBySocket(int iSocket, sockaddr_in& addr);

     int InitSignal();

 
public:
	CPublicFun* m_pFun;
    CMsgManage* m_pManage;
private:
    int     m_nEpollFd;
    int     m_nSock;
    pthread_t   m_thListenThreadId;

	
};














#endif
