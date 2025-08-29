
#include"IMsgBase.h"



bool IMsgBase::ExecuteMysql(const char* szSql)
{
    int nRet = 0;
   if(m_mySqlEncap)
   {
       nRet = m_mySqlEncap->ModifyQuery(szSql);
       if(nRet != 0)
       {
           printf("SelectMysql() failed! sql:%s\n ", szSql);
           return false;
       }
   }
    return true;
}
