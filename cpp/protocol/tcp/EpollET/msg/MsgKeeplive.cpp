#include "MsgKeeplive.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"MsgManage.h"


CMsgKeeplive::CMsgKeeplive()
{
    m_usBodyLen = 20;
    //m_uMsgType = ENU_MARS_MSG_KEEPLIVE;
}

CMsgKeeplive::~CMsgKeeplive()
{

}

void CMsgKeeplive::Deal(const USERDATAINFO& Data)
{
   // USERDATAINFO usrInfo;
    m_usrSendData.iSocket = Data.iSocket;
    m_usrSendData.nMsgLen = m_usBodyLen;

    m_usrSendData.usrPackage = Data.usrPackage;
     m_usrSendData.usrPackage.nCmd = htonl(Data.usrPackage.nCmd);

      printf("\n[Recv & Send]   [Msg:   Keepllive]  ,  Socket:%d", Data.iSocket);


    CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

    CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

    return ;
}
