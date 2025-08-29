#include"public/PublicFun.h"
#include"MsgDispose.h"
#include"usr/UserManage.h"



bool CMsgDispose::Init(int fd,  sockaddr_in addr)
{
    m_nFd = fd;
    m_addr = addr;
    pthread_mutex_init(&m_mtxTask, NULL);
    pthread_mutex_init(&m_mtxDuqueRecv, NULL);

    m_msgManage.Init();
    return true;
}
CMsgDispose::~CMsgDispose()
{
     pthread_mutex_destroy(&m_mtxTask);
    pthread_mutex_destroy(&m_mtxDuqueRecv);
}


bool CMsgDispose::Read()
{
    int nRecv = recv(m_nFd, &m_usrRecv.usrPackage, MARS_MSG_MAX_RECVDATA, 0);
    if(nRecv > 0)
    {
        m_usrRecv.nMsgLen = nRecv;
        m_msgManage. PrintData((char*)&m_usrRecv.usrPackage, nRecv);
        pthread_mutex_lock(&m_mtxDuqueRecv);
        m_msgDuqueRecv.push_back(m_usrRecv);
        pthread_mutex_unlock(&m_mtxDuqueRecv);
    }

    //printf("CMsgDispose::Read()   threadID:%d, nRecv:%d \n",pthread_self(), nRecv);


    return (nRecv > 0) ? true: false;
}

bool CMsgDispose::Write()
{
    //printf("[ Enter ]  CMsgDispose::Write()   ++++++\n" );
    pthread_mutex_lock(&m_msgManage.m_mtxMsgDuqueSend);
     int nSize = m_msgManage.m_msgDuqueSend.size();
    pthread_mutex_unlock(&m_msgManage.m_mtxMsgDuqueSend);

    //printf("[ Enter ]  CMsgDispose::Write()   nSize:%d\n", nSize );
    if(nSize == 0)
    {
        printf("[ INFO ]CMsgDispose::Write() msgDuqueSend nSize:%d\n", nSize);
          return true;
    }


    int nSend = 1;

    for(int i = 0; i < nSize; ++i)
    {
        pthread_mutex_lock(&m_msgManage.m_mtxMsgDuqueSend);
         m_usrSend = m_msgManage.m_msgDuqueSend.front();
        pthread_mutex_unlock(&m_msgManage.m_mtxMsgDuqueSend);

        nSend = send(m_usrSend.fd, &m_usrSend.usrPackage,  m_usrSend.nMsgLen, MSG_NOSIGNAL);

        printf("CMsgDispose::Write()   fd:%d, nSend:%d, cmd:%d \n",  m_usrSend.fd , nSend, ntohl(m_usrSend.usrPackage.nCmd));

         m_msgManage.MsgDequeSendPopFront();

         if(nSend < 0)
         {
              printf("[ ERROR ]  CMsgDispose::Write()  send  errno:%d\n",errno );
               pthread_mutex_lock(&m_msgManage.m_mtxMsgDuqueSend);
              m_msgManage.m_msgDuqueSend.clear();
              pthread_mutex_unlock(&m_msgManage.m_mtxMsgDuqueSend);

             break;
         }
    }

    //printf("nSend:%d", nSend);

    return (nSend > 0) ? true: false;
}

void CMsgDispose::Process()
{
     //printf("[ Enter ]  CMsgDispose::Process()  +++++\n" );

     pthread_mutex_lock(&m_mtxTask);

     //printf("[ Enter ]  CMsgDispose::Process()   Enter Recv+++++\n" );

     pthread_mutex_lock(&m_mtxDuqueRecv);
      int nSize = m_msgDuqueRecv.size();
      pthread_mutex_unlock(&m_mtxDuqueRecv);

       // printf("[ Enter ]  CMsgDispose::Process()   nSize:%d\n", nSize );

      while(nSize > 0)
      {
          pthread_mutex_lock(&m_mtxDuqueRecv);
           m_usrData = m_msgDuqueRecv.front();
           m_msgDuqueRecv.pop_front();
          pthread_mutex_unlock(&m_mtxDuqueRecv);

          m_usrData.fd = m_nFd;
          m_msgManage.Deal(m_usrData);
          Write();

          nSize =  m_msgDuqueRecv.size();
      }

   pthread_mutex_unlock(&m_mtxTask);

     //printf("[ Leave ]  CMsgDispose::Process()  ------\n" );
    return ;
}
