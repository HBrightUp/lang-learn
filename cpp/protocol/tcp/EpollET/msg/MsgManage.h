#ifndef __EXPLAINPACK_H__
#define __EXPLAINPACK_H__
#include "../public/PublicFun.h"
#include <pthread.h>
#include "IMsgBase.h"

#include<deque>
#include<vector>



class CMsgManage
{
private:
    CMsgManage();
    ~CMsgManage();
    void Init();


public:
    static CMsgManage* GetInstance();

    void Print(char* pData, uint32_t nLen);


    void MsgDequeRecvPush( USERDATAINFO& msgData);
    void  MsgDequeRecvPopFront( );

    void MsgDequeSendPush(USERDATAINFO& msgData);
    void MsgDequeSendPopFront();

    static void* MarsMsgDeque(void* arg);

    IMsgBase* GetMsgPtr(MarsMsgTypePos enType);

private:
    IMsgBase* FindMsgPtr(MarsMsgTypePos msgType);




private:

	char* m_pPack;
    std::deque<USERDATAINFO> m_msgDuqueRecv;
    pthread_mutex_t m_mtxMsgDuqueRecv;

    pthread_mutex_t m_mtxMsgDuqueSend;

    std::vector<ST_MSGPTR> m_vcMsgPtr;
    pthread_t m_threadMsg;
public:
    std::deque<USERDATAINFO> m_msgDuqueSend;
};



















#endif // __EXPLAINPACK_H__
