#ifndef _PUBLICFUN_H_
#define _PUBLICFUN_H_
#include <stdint.h>
#include<iostream>
#include <algorithm>
#include<iterator>
#include<cstring>
#include<set>
#include<deque>
#include<stdio.h>
#include <sstream>
#include <unistd.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include <time.h>
#include"../mysql/MysqlCom.h"
#include"ServerConfig.h"
#include"usr/UserManage.h"
#include"errorcode.h"


using namespace std;

#define MAX_BUFFER_SIZE  1024*1024

enum EnFdCtl
{
    ENU_FD_ADD = 1,
    ENU_FD_MOD,
    ENU_FD_DEL,
};


class CExplainPack;

class CPublicFun
{
public:
	CPublicFun();
	~CPublicFun();
	static CPublicFun* GetInstance();

public:

    void PrintLog(const char* pData);
    bool GetAddressBySocket(int fd, sockaddr_in& addr);
    int  EpollCtlFd( int epollfd, int fd, EnFdCtl Ctl, uint32_t events = 0);
    int SetNonblock(int fd);
    void StopConnect(int fd);

    bool Init();

    void GetSystemTime(string& str);

private:



public:
    int     m_nEpollFd;
    CMysqlCom m_sqlCom;
     CServerConfig m_ServerConfig;
     map<string,string> m_mapMsgOffline;
     pthread_mutex_t m_mtxOffline;



private:


};


#endif // _PUBLICFUN_H_
