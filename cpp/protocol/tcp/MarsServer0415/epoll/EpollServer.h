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
#include"threadpool.h"
#include "../public/PublicFun.h"
#include"../msg/MsgManage.h"
#include"epoll/MsgDispose.h"




#define MAX_SOCKFD_CONNECT 10000


class CEpollServer
{
protected:
    CEpollServer();
    ~CEpollServer();

public:
    static CEpollServer* GetInstance();
    bool InitServer();
    void Run();
 
public:
	CPublicFun* m_pFun;
    CMsgManage* m_pManage;
private:
    int     m_nEpollFd;
    int     m_nListenFd;
    pthread_t   m_thListenThreadId;
    CMsgDispose* m_pMsgPose;
    threadpool< CMsgDispose>* m_pThreadPool;
};














#endif
