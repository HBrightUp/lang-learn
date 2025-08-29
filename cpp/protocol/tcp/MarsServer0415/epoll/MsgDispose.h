#ifndef CMSGDISPOSE
#define CMSGDISPOSE
#include"./public/PublicFun.h"
#include"msg/MsgInfo.h"
#include"./msg/MsgManage.h"

class CMsgDispose
{
public:

    bool Init(int fd,  sockaddr_in addr);

    bool Read();
    bool Write();
    void Process();
   // void StopConnect();
public:
    ~CMsgDispose();



public:
    int m_nFd;
    sockaddr_in m_addr;

private:
    USERDATAINFO m_usrRecv;
    USERDATAINFO m_usrSend;
    USERDATAINFO m_usrData;
    CMsgManage  m_msgManage;

    pthread_mutex_t m_mtxTask;

    std::deque<USERDATAINFO> m_msgDuqueRecv;
    pthread_mutex_t m_mtxDuqueRecv;
};







#endif // CMSGDISPOSE

