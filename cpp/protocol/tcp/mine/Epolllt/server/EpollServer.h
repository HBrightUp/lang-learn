#ifndef _EPOLLSERVER_H_
#define _EPOLLSERVER_H_

#include<sys/epoll.h>
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



#define MAX_SOCKFD_CONNECT 65535
#define MAX_BUFFER_SIZE  1024*1024

class CEpollServer
{
public:
    CEpollServer();
    ~CEpollServer();

    bool InitServer(const char* strIP, int nPort);
    void Listen();
    static void ListenThread(void* lpParam);
    void Run();

	void PrintData(char* pData, int nLen);
 

private:
    int     m_nEpollFd;
    int     m_nSock;
    pthread_t   m_thListenThreadId;

};














#endif
