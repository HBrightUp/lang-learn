#include <iostream>
#include "MysqlCom.h"
#include "CEncapMysql.h"

using namespace std;
using namespace EncapMysql;

int main()
{
    MYSQLCONN mysqlConn;
    //mysqlConn.strIP = "127.0.0.1";
    mysqlConn.strIP = "pc.che.ba";

    mysqlConn.nPort = 3306;
    mysqlConn.strUsr = "root";

    //mysqlConn.strPwd = "root";
    mysqlConn.strPwd = "4GOXPBu8X71wVJS7";

    mysqlConn.nConnNum = 3;
    //mysqlConn.strDBName = "MarsDB";
     mysqlConn.strDBName = "marsdatabase";

    //int CMysqlCom::Init(string &strIP,  int32_t nPort, string &strUsr, string &strPwd, string& strDBName, int32_t nConnNum)

    CMysqlCom  mysql;
    mysql.Init(mysqlConn.strIP, mysqlConn.nPort, mysqlConn.strUsr, mysqlConn.strPwd, mysqlConn.strDBName, mysqlConn.nConnNum );

    CEncapMysql* pMysql =  static_cast<CEncapMysql*>(mysql.GetOneConn());

    if(pMysql == NULL)
    {
            printf(" create one connect failed!\n");
            return 0;
    }
    string strSql = "select * from tb_usrinfo";
    int nSize = pMysql->SelectQuery(strSql.data());


    return 0;
}

