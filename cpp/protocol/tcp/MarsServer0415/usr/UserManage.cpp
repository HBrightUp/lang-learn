#include "../public/PublicFun.h"
#include "UserManage.h"


class  CUsrLoginFind
{
public:

    CUsrLoginFind( int  fd)
    {
        m_nFd = fd;
    }

    bool operator () (const MAPUSRLOGIN::value_type  & pair)
    {
        return  m_nFd  == pair.second.fd;
    }

private:
    int   m_nFd;

};


CUsrManage::CUsrManage()
{
    pthread_mutex_init(&m_mtxUsrLogin,NULL);
}

CUsrManage::~CUsrManage()
{
    pthread_mutex_destroy(&m_mtxUsrLogin);
}

CUsrManage* CUsrManage::GetInstance()
{
    static CUsrManage* pUsr = 0;
    if(!pUsr)
    {
        pUsr = new CUsrManage();
    }

    return  pUsr;
}

bool  CUsrManage::AddUsr(uint32_t uKey, const STUSRLOGIN& stLogin)
{
    pthread_mutex_lock(&m_mtxUsrLogin);
    m_mapUsrLogin[uKey] = stLogin;
    m_setUsrOnline.insert(uKey);
    pthread_mutex_unlock(&m_mtxUsrLogin);

    PrintUsr();

    return  true;
}

bool CUsrManage::DelUsrBySocket(const int iSockt)
{
    if(iSockt <= 0)
        return false;

    MAPUSRLOGIN::iterator iter;
    pthread_mutex_lock(&m_mtxUsrLogin);

    iter = find_if(m_mapUsrLogin.begin(), m_mapUsrLogin.end(),  CUsrLoginFind(iSockt));
     if(iter != m_mapUsrLogin.end())
     {
         m_mapUsrLogin.erase(iter);
         m_setUsrOnline.erase(iter->first);
     }

    pthread_mutex_unlock(&m_mtxUsrLogin);


     PrintUsr();

    return true;
}
 bool CUsrManage::DelUsrByUsrID(const uint32_t usrID)
{
     pthread_mutex_lock(&m_mtxUsrLogin);
     m_mapUsrLogin.erase(usrID);
     m_setUsrOnline.erase(usrID);
     pthread_mutex_unlock(&m_mtxUsrLogin);

     PrintUsr();

     return true;
}

 int CUsrManage::GetUsrSocket(uint32_t usrID)
{
     if(usrID <= 0)
         return -1;

     //PrintUsr();

     return m_mapUsrLogin[usrID].fd;
 }

 uint32_t CUsrManage::GetUsrUserId(int fd)
 {
     uint32_t userid = 0;

     MAPUSRLOGIN::iterator iter;
     pthread_mutex_lock(&m_mtxUsrLogin);

     iter = find_if(m_mapUsrLogin.begin(), m_mapUsrLogin.end(),  CUsrLoginFind(fd));
      if(iter != m_mapUsrLogin.end())
      {
          userid = iter->first;
      }

     pthread_mutex_unlock(&m_mtxUsrLogin);

     return userid;
 }

 void CUsrManage::PrintUsr()
 {
     if(m_mapUsrLogin.size() ==0)
     {
         printf("\nONLINE:     NO USER\n");

         return ;
     }

     MAPUSRLOGIN::const_iterator iter;
     for(iter = m_mapUsrLogin.begin(); iter != m_mapUsrLogin.end(); ++iter)
     {
         printf("\nONLINE:  usrID:   %d,  fd:   %d\n",iter->first, iter->second.fd);
     }
 }

