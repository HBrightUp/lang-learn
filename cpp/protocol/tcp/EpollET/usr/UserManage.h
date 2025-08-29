#ifndef CUSERMANAGE
#define CUSERMANAGE
#include "../public/PublicFun.h"

struct STUSRLOGIN
{
    //unsigned int usrID;
    int iSocket;
};

typedef std::map<uint32_t,  STUSRLOGIN>  MAPUSRLOGIN;


class 	CUsrManage
{
  private:
    CUsrManage();
    ~CUsrManage();

    //static bool mapFindLoginID(const  STUSRLOGIN& stLogin);
    void PrintUsr();


public:
    static CUsrManage* GetInstance();
    bool AddUsr(uint32_t uKey, const STUSRLOGIN& stLogin);
    bool DelUsrBySocket(const int nSockt);
    bool DelUsrByUsrID(const uint32_t  usrID);

    int GetUsrSocket(uint32_t usrID);



public:
    set<uint32_t> m_setUsrOnline;      //   在线用户
 MAPUSRLOGIN m_mapUsrLogin;      //  在线用户相关资源与信息

private:

    pthread_mutex_t m_mtxUsrLogin;

};















#endif // CUSERMANAGE

