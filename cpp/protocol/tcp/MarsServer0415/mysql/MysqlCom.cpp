#include "MysqlCom.h"
#include "EncapMysql.h"

using namespace EncapMysql ;

CMysqlCom::CMysqlCom()
{
    m_nConnNum = 0;
}


CMysqlCom::~CMysqlCom()
{

}

int CMysqlCom::Init(string &strIP,  int32_t nPort, string &strUsr, string &strPwd, string& strDBName, int32_t nConnNum)
{
        m_strIP  = strIP;
        m_nPort = nPort;
        m_strUsr     = strUsr;
        m_strPwd      = strPwd;
        m_strDBName   = strDBName;
        m_nConnNum = nConnNum;

        CEncapMysql* pEM = NULL;
        int nRet = 0;

        for(int i = 0;  i < nConnNum;  i++)
        {
           pEM = (CEncapMysql*)this->CreateOneConn();
            if(!pEM )
                     return -1;

          m_vectorConn.push_back(pEM);
          m_mapVI[pEM] = i;
      }

        return nRet;
}



void* CMysqlCom::CreateOneConn()
{
    try
       {
         CEncapMysql*  pEM = new CEncapMysql();
         if(pEM == NULL)
         {
                printf("pEM == NULL**\r\n");
                return NULL;
         }

         int nRet = pEM->Connect(m_strIP.c_str(),   m_strUsr.c_str(), m_strPwd.c_str(), m_strDBName.c_str(), m_nPort);
         if(nRet != 0)
         {
                printf("pEM->Connect fail**\r\n");
                return NULL;
         }

          pEM->SetIdle();

              return pEM;
      }
      catch(...)
      {
         printf("createOneConn  exception**\r\n");
         return NULL;
      }

}

void* CMysqlCom::GetOneConn()
{
    //CGuard  guard(m_sMutex);

      for(uint32_t i = 0;  i < m_nConnNum; ++i)
      {
          CEncapMysql* pEM = (CEncapMysql*)m_vectorConn[i];
            if( pEM->IsIdle())
            {
               pEM->SetUsed();
              return pEM;
            }
      }

      //可能访问MYSQL的用户较多，连接池中已无空闲连接了。只要总连接数没有超限，就新建一个连接。
      if(m_nConnNum < MYSQL_CONN_NUM_MAX_VALUE)
      {
         CEncapMysql* pEM = (CEncapMysql*)this->CreateOneConn();
            if(!pEM )
              return NULL;




          m_vectorConn.push_back(pEM);
          m_mapVI[pEM] = m_nConnNum++;

          printf("CMysqlCom::GetOneConn()  m_nConnNum:%d,   m_vectorConn.size:%d,    m_mapVI.size:%d", m_nConnNum, m_vectorConn.size(), m_mapVI.size());
      }

      return NULL;
}

void CMysqlCom::ReleaseOneConn(void *pConn)
{
        map<void*, int>::iterator it;

      //CGuard  guard(m_sMutex);

      it = m_mapVI.find(pConn);
      if(it == m_mapVI.end())
      {
          printf("retOneConn  fail***\n");
          return;
      }
      int nInx = it->second;

      CEncapMysql* pEM = (CEncapMysql*) m_vectorConn[nInx];
      pEM->SetIdle();

       printf("retOneConn  succ!\n");
}
void CMysqlCom::CheckConn()
{

}
