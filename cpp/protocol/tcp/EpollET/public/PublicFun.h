#ifndef _PUBLICFUN_H_
#define _PUBLICFUN_H_
#include <stdint.h>
#include<iostream>
#include <algorithm>
#include<iterator>
#include<cstring>
#include<set>
#include<stdio.h>
#include <signal.h>
#include<sys/epoll.h>
#include"../mysql/MysqlCom.h"
#include"ServerConfig.h"


using namespace std;

#define MAX_BUFFER_SIZE  1024*1024

class CExplainPack;

class CPublicFun
{
public:
	CPublicFun();
	~CPublicFun();
	static CPublicFun* GetInstance();

public:
    void AwakenWriteEvent(int nSocket);
    void AwakenReadEvent(int nSocket);
     void AwakenDelEvent(int nSocket);
    void PrintLog(const char* pData);

    bool Init();

    int64_t GetSystemTime();
private:



public:
    int     m_nEpollFd;
    CMysqlCom m_sqlCom;
     CServerConfig m_ServerConfig;

private:
    //MYSQLCONN m_sqlConfig;

};


#endif // _PUBLICFUN_H_
