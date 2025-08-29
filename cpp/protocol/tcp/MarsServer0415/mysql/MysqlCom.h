#ifndef _MYSQLCOM_H_
#define _MYSQLCOM_H_
#include<iostream>
#include <cstring>
#include <stdlib.h>
#include<vector>
#include <map>
#include<stdint.h>

using namespace std;

#define  MYSQL_CONN_NUM_MAX_VALUE   500

typedef struct st_mysql_conn
{
    string strIP;
    int32_t nPort;
    string strUsr;
    string strPwd;
    string strDBName;
    int32_t nConnNum;
}MYSQLCONN,*PMYSQLCONN;


class CMysqlCom
{
public:
    CMysqlCom();
    ~CMysqlCom();

public:
    int Init(string &strIP,  int32_t nPort, string &strUsr, string &strPwd, string& strDBName, int32_t nConnNum);
    void* GetOneConn();
    void  ReleaseOneConn(void* pConn);
    void  CheckConn();
    void* CreateOneConn();




public:
    string m_strIP;
    uint32_t  m_nPort;
    string m_strUsr;
    string m_strPwd;
    string m_strDBName;
    uint32_t   m_nConnNum;

private:
    vector<void*>  m_vectorConn;
    map<void*, int> m_mapVI; //  从连接的地址，快速找到索引，便于存放到m_vectorConn中。


};

















#endif
