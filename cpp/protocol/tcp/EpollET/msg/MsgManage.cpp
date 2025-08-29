#include<iostream>
#include<vector>
#include "MsgManage.h"
#include "../protobuf//chat.pb.h"
#include "IMsgBase.h"
#include "MsgKeeplive.h"
#include "MsgChatPrivate.h"
#include "MsgChatGroup.h"
#include "MsgPush.h"
#include "MsgLogin.h"
#include"MsgContactReq.h"
#include"MsgContactRsp.h"
#include <unistd.h>
#include <arpa/inet.h>


using namespace std;

CMsgManage::CMsgManage()
{
    pthread_create(&m_threadMsg, NULL, MarsMsgDeque, this);
}

CMsgManage::~CMsgManage()
{
	if(m_pPack != NULL)
	{
		delete m_pPack;
		m_pPack = NULL;
	}

    pthread_mutex_destroy(&m_mtxMsgDuqueRecv);
    pthread_mutex_destroy(&m_mtxMsgDuqueSend);
	
}

void CMsgManage::Init()
{
    ST_MSGPTR stMsg;

    stMsg.pos = ENU_MARS_MSG_CHAT_PRIVATE_POS;
    stMsg.Msgptr = new CMsgChatPrivate();
    m_vcMsgPtr.push_back(stMsg);

    stMsg.pos = ENU_MARS_MSG_KEEPLIVE_POS;
    stMsg.Msgptr = new CMsgKeeplive();
    m_vcMsgPtr.push_back(stMsg);

    stMsg.pos = ENU_MARS_MSG_PUSH_POS;
    stMsg.Msgptr = new CMsgPush();
    m_vcMsgPtr.push_back(stMsg);

    stMsg.pos = ENU_MARS_MSG_LOGIN_POS;
    stMsg.Msgptr = new CMsgLogin();
    m_vcMsgPtr.push_back(stMsg);

    stMsg.pos = ENU_MARS_MSG_CONTACT_REQ_A_POS;
    stMsg.Msgptr = new CMsgContactAdd;
    m_vcMsgPtr.push_back(stMsg);

    stMsg.pos = ENU_MARS_MSG_CONTACT_RSP_POS;
    stMsg.Msgptr = new CMsgContactRsp;
    m_vcMsgPtr.push_back(stMsg);

    stMsg.pos = ENU_MARS_MSG_CHAT_GROUP_POS;
    stMsg.Msgptr = new CMsgChatGroup;
    m_vcMsgPtr.push_back(stMsg);

    pthread_mutex_init(&m_mtxMsgDuqueRecv, NULL);
    pthread_mutex_init(&m_mtxMsgDuqueSend, NULL);


}

IMsgBase* CMsgManage::FindMsgPtr(MarsMsgTypePos  msgType)
{
     IMsgBase* pMsgBase = 0;

    vector<ST_MSGPTR>::iterator it;
    for(it = m_vcMsgPtr.begin(); it != m_vcMsgPtr.end(); ++it)
    {
        if(it->pos == msgType)
        {
            pMsgBase = it->Msgptr;
        }
    }

    return pMsgBase;
}


CMsgManage* CMsgManage::GetInstance()
{
    static CMsgManage* pMsg = NULL;
    if(!pMsg)
    {
        pMsg = new CMsgManage();
        pMsg->Init();
    }

    return pMsg;
}

 void CMsgManage::MsgDequeRecvPush( USERDATAINFO& msgData)
 {

     pthread_mutex_lock(&m_mtxMsgDuqueRecv);


     m_msgDuqueRecv.push_back(msgData);
     pthread_mutex_unlock(&m_mtxMsgDuqueRecv);

     return ;
 }
  void  CMsgManage::MsgDequeRecvPopFront( )
  {
       //printf(" CMsgManage::MsgDequeRecvPopFront +++");
      pthread_mutex_lock(&m_mtxMsgDuqueRecv);
      m_msgDuqueRecv.pop_front();
      pthread_mutex_unlock(&m_mtxMsgDuqueRecv);
       //printf(" CMsgManage::MsgDequeRecvPopFront ---");
  }


  void CMsgManage::MsgDequeSendPush(USERDATAINFO& msgData)
  {
       //printf(" CMsgManage::MsgDequeSendPush +++");
      pthread_mutex_lock(&m_mtxMsgDuqueSend);
      m_msgDuqueSend.push_back(msgData);
      pthread_mutex_unlock(&m_mtxMsgDuqueSend);
      // printf(" CMsgManage::MsgDequeSendPush ---");

  }
  void CMsgManage::MsgDequeSendPopFront()
  {
       //printf(" CMsgManage::MsgDequeSendPopfront +++");
      pthread_mutex_lock(&m_mtxMsgDuqueSend);
      m_msgDuqueSend.pop_front();
      pthread_mutex_unlock(&m_mtxMsgDuqueSend);

      //printf(" CMsgManage::MsgDequeSendPopfront---");

  }

IMsgBase* CMsgManage::GetMsgPtr(MarsMsgTypePos enType)
{
    if(enType < 0)
    {
        return NULL;
    }

    std::vector<ST_MSGPTR>::const_iterator iter;

    for(iter = m_vcMsgPtr.begin(); iter != m_vcMsgPtr.end(); ++iter)
    {
        if(iter->pos == enType)
        {
                return  iter->Msgptr;
        }
    }

    return NULL;
}


  void* CMsgManage::MarsMsgDeque(void* arg)
{

    CMsgManage* pThis = static_cast<CMsgManage*>(arg);

    deque<USERDATAINFO>::iterator iter;

    IMsgBase* pMsgBase = NULL;

    USERDATAINFO msgInfo;

    while(true)
    {
        size_t uSize = pThis->m_msgDuqueRecv.size();

        for(size_t i = 0; i < uSize; ++i)
        {
             pthread_mutex_lock(&pThis->m_mtxMsgDuqueRecv);
            msgInfo = pThis->m_msgDuqueRecv.front();
             pthread_mutex_unlock(&pThis->m_mtxMsgDuqueRecv);
            msgInfo.usrPackage.nCmd = ntohl(msgInfo.usrPackage.nCmd);

            uint32_t cmd = msgInfo.usrPackage.nCmd;

            switch(cmd)
             {
                    case ENU_MARS_MSG_CHAT_PRIVATE:
                    {
                         pMsgBase = pThis->FindMsgPtr(ENU_MARS_MSG_CHAT_PRIVATE_POS);
                         break;
                     }
                    case ENU_MARS_MSG_KEEPLIVE:
                    {
                         pMsgBase = pThis->FindMsgPtr(ENU_MARS_MSG_KEEPLIVE_POS);
                         break;
                    }
                   case ENU_MARS_MSG_LOGIN:
                    {
                        pMsgBase = pThis->FindMsgPtr(ENU_MARS_MSG_LOGIN_POS);
                         break;
                    }

                 case ENU_MARS_MSG_CONTACT_REQ_A:
                 {
                      pMsgBase = pThis->FindMsgPtr(ENU_MARS_MSG_CONTACT_REQ_A_POS);
                        break;
               }

                 case ENU_MARS_MSG_CONTACT_RSP:
                {
                     pMsgBase = pThis->FindMsgPtr(ENU_MARS_MSG_CONTACT_RSP_POS);
                     break;
                }

               case ENU_MARS_MSG_CHAT_GROUP:
              case ENU_MARS_MSG_SYSTEM_PUSH:
              {
                     pMsgBase= pThis->FindMsgPtr((ENU_MARS_MSG_CHAT_GROUP_POS));
                     break;
              }

                    default:
                     {
                        printf("unknown message,cmd:%d", cmd);
                         pThis->MsgDequeRecvPopFront();
                         pMsgBase = NULL;

                         break;
                     }
             }

            if(pMsgBase)
            {
                 pMsgBase->SetCmdId(cmd);
                pMsgBase->Deal(msgInfo);
                if(cmd != ENU_MARS_MSG_KEEPLIVE)
                {
                    pThis->MsgDequeRecvPopFront();
                }
            }

            if(cmd == ENU_MARS_MSG_KEEPLIVE)
            {

                //pMsgBase = pThis->FindMsgPtr(ENU_MARS_MSG_PUSH_POS);
                //pMsgBase->Deal(msgInfo);

                pThis->MsgDequeRecvPopFront();

            }

        }
        usleep(1000 * 100);
    }
}


