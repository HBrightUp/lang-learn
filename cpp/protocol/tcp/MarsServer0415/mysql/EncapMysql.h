#ifndef ENCAPSULATION_MYSQL_H_
#define ENCAPSULATION_MYSQL_H_

#include <iostream>
#include <cassert>
#include <set>
#include <sys/shm.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include<set>
#include <time.h>
#include <stdlib.h>
#include <memory>
#include <iconv.h>
#include <dlfcn.h>
#include <mysql/mysql.h>
#include <cstring>

using namespace std;

//mysql查询结果获取方式
enum en_ResultType
{
    ENU_SELECTDATA_VECTOR = 1,
    ENU_SELECTDATA_SET,
};

#define ERRMSG1(fmt,...)  ; sprintf(m_szErrMsg, fmt, __VA_ARGS__);
#define ERRMSG2(fmt,args...)  ; sprintf(m_szErrMsg, "[%s 第 %d 行 ]; " fmt"\r\n" , __FILE__, __LINE__, ##args);
namespace EncapMysql
{

class CEncapMysql
{
    typedef map<unsigned int, string> MapFieldNameIndex;
public:
    CEncapMysql();
    ~CEncapMysql();
    void SetUsed();
    void SetIdle();
    bool IsIdle();       //  Idle:true
    int Connect(const char* szDbIp, const char* szUser, const char* szPassword,const char* szDBName, uint nPort );
    void CloseConnect();
    int SelectQuery(const char* szSQL, en_ResultType enType = ENU_SELECTDATA_VECTOR);
    int ModifyQuery(const char* szSQL);
    const char* GetErrMsg();
    char** FetchRow();
    char* GetField(const char* szFieldName);
    vector<string>&  GetSelectResult();

private:

    bool IsConnected();
    void SetConnected(bool bTrueFalse);
    char* GetField(unsigned int iFieldIndex);
    void FreePreResult();
    int ReConnect();
    void SaveParam(const char* szDbIp, const char* szUser, const char* szPassword, const char* szDBName,unsigned int nPort);

public:
    bool m_bConnected;    //数据库连接了吗?   true--已经连接;  false--还没有连接
    char m_szErrMsg[1024]; //函数出错后, 错误信息放在此处
    unsigned int m_iFields; //字段个数
    MapFieldNameIndex m_mapFieldNameIndex; //是一个map,  key是字段名,  value是字段索引
public:
    MYSQL m_connection; //连接
    MYSQL_RES* m_result; //结果集指针
    MYSQL_ROW m_row; //一行,  typedef char **MYSQL_ROW;

    set<int32_t> m_setSelectResult;

private:
    bool m_bUseIdle;    // true: use;   false:idle
    string m_sDbIp; //数据库服务器IP
    string m_sUser; //用户名
    string m_sPassword; //口令
    string m_sDBName;
    unsigned int m_nPort;
    vector<string> m_vcSelectResult;
};

} //end of namespace  EncapMysql

#endif
