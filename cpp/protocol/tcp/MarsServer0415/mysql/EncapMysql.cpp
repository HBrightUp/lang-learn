
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <set>
#include <map>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include "EncapMysql.h"
#include <mysql/mysql.h>
#include<mysql/errmsg.h>

using namespace std;
using namespace EncapMysql;

CEncapMysql::CEncapMysql()
{
    SetConnected(false);
    m_result = NULL;
    mysql_init(&m_connection);
    m_vcSelectResult.reserve(1000);
}
CEncapMysql::~CEncapMysql()
{
    FreePreResult();
    CloseConnect();
}

int CEncapMysql::Connect(const char* szDbIp, const char* szUser,const char* szPassword, const char* szDBName, uint nPort)
{
    SaveParam(szDbIp, szUser, szPassword, szDBName, nPort);

    if (IsConnected())
        return 0;

    if (mysql_real_connect(&m_connection, szDbIp, szUser, szPassword, szDBName, nPort,NULL, 0) == NULL)
    {
       // ERRMSG2("%s", mysql_error(&m_connection));
        char buf[64];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf),"mysql connect error:%s", mysql_error(&m_connection));
        std::cout << buf << endl;

        return -1;
    }

    //printf("[ mysql ] connnect to %s [user:%s] success!\n", szDbIp, szUser);

    SetConnected(true);

    mysql_set_character_set(&m_connection, "uft8");

    const char* str = "call sp_safe_updates()";
    int ret = mysql_real_query(&m_connection, str, strlen(str));
    if(ret != 0)
    {
         printf("call sp_safe_updates failed!ret:%d\n", ret);
    }

    return 0;
}

void CEncapMysql::CloseConnect()
{
    //不论m_connection曾经是否连接过， 这样关闭都不会有问题
    mysql_close(&m_connection);
    SetConnected(false);
}

int CEncapMysql::SelectQuery(const char* szSQL, en_ResultType enType)
{
    if (szSQL == NULL)
    {
        ERRMSG2("%s", "szSQL==NULL");
        return -1;
    }

    if (!IsConnected()) {
        ERRMSG2("%s", "还没有建立连接");
        return -2;
    }
    try //这些语句与连接有关，出异常时就重连
    {
        if (mysql_set_character_set(&m_connection, "utf8"))
        {
            printf("New client character set failed: %s\n", mysql_character_set_name(&m_connection));
        }

        //查询
        int nRet = mysql_real_query(&m_connection, szSQL, strlen(szSQL));

        if (nRet !=  0)
        {
            ERRMSG2("%s", mysql_error(&m_connection));
            printf("%s", mysql_error(&m_connection));
            printf("ReConnect()  is called, select111  !!!***\r\n");
            int nRet = ReConnect();
            if (nRet != 0)
                return -3;

             nRet = mysql_real_query(&m_connection, szSQL, strlen(szSQL));
            if ( nRet!= 0)
                return -33;

        }
        //释放上一次的结果集
        FreePreResult();
        //取结果集
        m_result = mysql_store_result(&m_connection);
        if (m_result == NULL) {
            ERRMSG2("%s", mysql_error(&m_connection));
            return -4;
        }
    } catch (...) {
        printf("ReConnect()  is called, select  !!!***\r\n");
        ReConnect();
        return -5;
    }
    //取字段的个数
    m_iFields = mysql_num_fields(m_result);

    //m_mapFieldNameIndex.clear();
    //取各个字段的属性信息
    MYSQL_FIELD *fields;
    fields = mysql_fetch_fields(m_result);

    //把字段名字和索引保存到一个map中
    for (unsigned int i = 0; i < m_iFields; i++) {
        m_mapFieldNameIndex[i] = fields[i].name;
    }

    MYSQL_ROW row = NULL;
    //是否自动获取查询数据至vector
    if(enType == ENU_SELECTDATA_VECTOR)
    {
            while(row = mysql_fetch_row(m_result))
            {
                for(unsigned int  i = 0; i < m_iFields; i++)
                {
                    m_vcSelectResult.push_back(row[i]);
                }
            }
    }
    else if(enType == ENU_SELECTDATA_SET )
    {
        while(row = mysql_fetch_row(m_result))
        {
            for(unsigned int  i = 0; i < m_iFields; i++)
            {
                m_setSelectResult.insert(atoi(row[i]));
            }
        }
    }
    else
    {

    }

    return 0;
}


int CEncapMysql::ModifyQuery(const char* szSQL)
{
    //如果查询串是空指针,则返回
    if (szSQL == NULL)
    {
        ERRMSG2("%s", "szSQL==NULL");
        return -1;
    }

    //如果还没有连接,则返回
    if (!IsConnected())
    {
        ERRMSG2("%s", "还没有建立连接");
        return -2;
    }

    try //这些语句与连接有关，出异常时就重连
    {
        //查询, 实际上开始真正地修改数据库
        if (mysql_real_query(&m_connection, szSQL, strlen(szSQL)) != 0)
        {
            printf("[ ERROR ] CEncapMysql::ModifyQuery(),errno:%d,m_szErrMsg:%s\n",errno, m_szErrMsg);
            ERRMSG2("%s", mysql_error(&m_connection));
            return -3;
        }
    } catch (...)
    {
        printf("ReConnect()  is called  ,modify!!!***\r\n");
         printf("[ ERROR ] CEncapMysql::ModifyQuery(),errno:%d,m_szErrMsg:%s\n",errno, m_szErrMsg);
        ReConnect();
        return -5;
    }

    return 0;
}

vector<string>&  CEncapMysql::GetSelectResult()
{
    return m_vcSelectResult;
}

char** CEncapMysql::FetchRow()
{
    //如果结果集为空,则直接返回空; 调用FetchRow之前, 必须先调用 SelectQuery(...)
    if (m_result == NULL)
        return NULL;

    //从结果集中取出一行
    m_row = mysql_fetch_row(m_result);
    return m_row;
}

char* CEncapMysql::GetField(unsigned int iFieldIndex)
{
    //防止索引超出范围

    if (iFieldIndex >= m_iFields)
        return NULL;
    return m_row[iFieldIndex];
}

void CEncapMysql::FreePreResult()
{
    m_vcSelectResult.clear();
    m_mapFieldNameIndex.clear();
    m_iFields = 0;

    if (m_result != NULL) {
        mysql_free_result(m_result);
        m_result = NULL;
    }
}

const char* CEncapMysql::GetErrMsg()
{
    return m_szErrMsg;
}

bool CEncapMysql::IsConnected()
{
    return m_bConnected;
}

void CEncapMysql::SetConnected(bool bTrueFalse)
{
    m_bConnected = bTrueFalse;
}

void CEncapMysql::SaveParam(const char* szDbIp, const char* szUser,const char* szPassword, const char* szDBName, unsigned int nPort)
{
    m_sDbIp = szDbIp; //数据库服务器IP
    m_sUser = szUser; //用户名
    m_sPassword = szPassword; //口令
    m_nPort = nPort;
    m_sDBName = szDBName;
}

int CEncapMysql::ReConnect()
{
    CloseConnect();

    //连接数据库
    if (mysql_real_connect(&m_connection, m_sDbIp.c_str(), m_sUser.c_str(),
            m_sPassword.c_str(),m_sDBName.c_str(),m_nPort, NULL, 0) == NULL) {
        ERRMSG2("%s", mysql_error(&m_connection));
        return -1;
    }

    SetConnected(true);

    const char* str = "call sp_safe_updates()";
    int ret = mysql_real_query(&m_connection, str, strlen(str));
    if(ret == 0)
    {
        printf("call sp_safe_updates success\n");
    }
    else
    {
        printf("call sp_safe_updates failed!ret:%d\n", ret);
    }
    return 0;
}

void CEncapMysql::SetUsed() {
    m_bUseIdle = true;
}

void CEncapMysql::SetIdle() {
    m_bUseIdle = false;
}

bool CEncapMysql::IsIdle() {
    return !m_bUseIdle;
}
