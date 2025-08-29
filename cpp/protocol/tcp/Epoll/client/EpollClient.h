#ifndef _EPOLLClIENT_H_
#define _EPOLLCLIENT_H_

#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

using namespace std;

#define MAX_SOCKFD_COUNT 65535
#define MAX_BUFFER_SIZE  128

typedef enum _EPOLL_USER_STATUS_EM
{
    FREE = 0,
    CONNECT_OK = 1,
    SEND_OK = 2,
    RECV_OK = 3,
}EPOLL_USER_STATUS_EM;

struct UserStatus
{
    EPOLL_USER_STATUS_EM iUserStatus;
    int iSockFd;
    char cSendbuff[1024];
    int iBuffLen;
    unsigned int uEpollEvents;
};

class CEpollClient
{
public:

    CEpollClient(int iUserCount, const char* pIP, int iPort);
    ~CEpollClient();
    int Run();

private:
    
    int ConnectToServer(int iUserId, const char *pServerIp, unsigned short uServerPort);
    int SendToServerData(int iUserId);
    int RecvFromServer(int iUserId, char* PRecvBuff, int iBuffLen);
    bool CloseUser(int iUserId);
    bool DelEpoll(int iSockFd);

private:
    
    int m_iUserCount;
    struct UserStatus* m_pAllUserStatus;
    int m_iEpollFd;
    int m_iSockFd_UserId[MAX_SOCKFD_COUNT];
    int m_iPort;
    char m_ip[128];








};

















#endif
