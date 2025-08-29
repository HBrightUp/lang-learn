#include "MsgPush.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"../msg/MsgManage.h"
#include "../protobuf/chat.pb.h"


CMsgPush::CMsgPush()
{
    m_usBodyLen = 0;
    //m_uMsgType = ENU_MARS_MSG_PUSH;
}

CMsgPush::~CMsgPush()
{

}

void CMsgPush::Deal(const USERDATAINFO& Data)
{
    MarsMsgChat::PushRsp msgPush;
    msgPush.set_msgtype(99);

    int32_t nTemp = 0;
    msgPush.set_errmsg(nTemp);

    msgPush.set_state(2012);

    string strTemp = "token_ 1240";
    msgPush.set_accesstoken(strTemp);

    strTemp = "userfrom";
    msgPush.set_userfrom(strTemp);

    strTemp = "userto";
    msgPush.set_userto(strTemp);

    strTemp = "text:write";
    msgPush.set_text(strTemp);

    strTemp = "headimage11";
    msgPush.set_headimage(strTemp);


    strTemp = "send name";
    msgPush.set_sendname(strTemp);

    strTemp = "senduserid:120";
    msgPush.set_senduserid(strTemp);

    strTemp = "converstionid";
    msgPush.set_converstionid(strTemp);

    msgPush.set_sendtype(11);

     USERDATAINFO usrInfo;

     msgPush.SerializeToArray(usrInfo.usrPackage.usrData, sizeof(usrInfo.usrPackage.usrData));
     usrInfo.nMsgLen = msgPush.ByteSize()  + MARS_MSG_HEAR_LEN;

    usrInfo.iSocket = Data.iSocket;

     nTemp = MARS_MSG_HEAR_LEN;
    usrInfo.usrPackage.nHeadLen = htonl(nTemp);

    nTemp = 200;
    usrInfo.usrPackage.nVersion = htonl(nTemp);

    usrInfo.usrPackage.nBodyLen = htonl(msgPush.ByteSize());

    nTemp = 0;
    usrInfo.usrPackage.nSeq = htonl(nTemp);

    nTemp = ENU_MARS_MSG_PUSH;
    usrInfo.usrPackage.nCmd = htonl(nTemp);

    usrInfo.iSocket = Data.iSocket;

    //CMsgManage::GetInstance()->m_msgDuqueSend.push_back(usrInfo);
    CMsgManage::GetInstance()->MsgDequeSendPush(usrInfo);
     CPublicFun::GetInstance()->AwakenWriteEvent(usrInfo.iSocket);

    return ;
}

int32_t CMsgPush::PushOffLineMsg(int32_t usrID, int nSocket)
{
    if(ExistContactAddMsg(usrID))
    {
        DealContactAddMsg();
    }




















#if 1
    memset(m_szSql, 0, sizeof(m_szSql));

    snprintf(m_szSql, sizeof(m_szSql),"SELECT A.msgid,A.fromUsrId,A.toUsrid,A.text,A.msgType,A.sendType,A.conversationId,B.name,B.head  FROM tb_msg  A, tb_usrinfo B\
                                      WHERE A.toUsrid = %d AND B.uid = %d", usrID,usrID);

     int nRet = 0;
     if(m_mySqlEncap)
     {
         nRet = m_mySqlEncap->SelectQuery(m_szSql);
         {
             if(nRet  == 0)
             {
                 SendMsgToB(nSocket);
             }
             else
             {
                  printf("GetConversationID failed!\n ");
                  return false;
             }
         }
     }
#endif
    return 0;
}

bool CMsgPush::SendMsgToB(int iSocket)
{
    vector<string>& vc = m_mySqlEncap->GetSelectResult();

    unsigned int  nMsgItem = vc.size() / m_mySqlEncap->m_iFields;


  MarsMsgChat::PushRsp msgRsp;

  uint32_t uTemp = 2;	//消息推送
  msgRsp.set_msgpushtype(uTemp);

  uTemp = 0;
  msgRsp.set_errmsg(uTemp);

  msgRsp.set_state(uTemp);

  string strTemp = "0";
  msgRsp.set_accesstoken(strTemp.c_str());

    //reserve
    msgRsp.set_senduserid(strTemp);










    //vector对应字段:   tb_msg(msgid,fromUsrId,toUsrid,text,msgType,sendType,conversationId),tb_usrinfo(name,head,)

    vector<string>::iterator iter = vc.begin();

    for(unsigned int i = 0; i < nMsgItem; ++i)
    {
      msgRsp.set_msgid(atoi(iter->c_str()));
      ++iter;
      msgRsp.set_userfrom(iter->c_str());
      ++iter;
      msgRsp.set_userto(iter->c_str());
      ++iter;
      msgRsp.set_text(iter->c_str());
      ++iter;

      msgRsp.set_msgtype(atoi(iter->c_str()));
      ++iter;

      msgRsp.set_sendtype(atoi(iter->c_str()));
      ++iter;
      msgRsp.set_converstionid(iter->c_str());
      ++iter;


      msgRsp.set_sendname(iter->c_str());
      ++iter;

      msgRsp.set_headimage(iter->c_str());
      ++iter;



      msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
      m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSendData.iSocket = iSocket;

     m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
     m_usrSendData.usrPackage.nSeq = 0;

     uTemp = ENU_MARS_MSG_PUSH;
     m_usrSendData.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
     m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

     uTemp = 200;
     m_usrSendData.usrPackage.nVersion = htonl(uTemp);

     CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

      CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

   }
     return true;
}

bool CMsgPush::ExistContactAddMsg(uint32_t userid)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT uid FROM tb_contacts WHERE cuid = %d AND status < 1", userid);

     int nRet = 0;
     if(m_mySqlEncap)
     {
         nRet = m_mySqlEncap->SelectQuery(m_szSql);
         {
             if(nRet  == 0)
             {
                vector<string>& vc = m_mySqlEncap->GetSelectResult();

                //是否有好友请求需要推送
                if(m_mySqlEncap->GetSelectResult().size() > 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }

             }
             else
             {
                  printf("GetConversationID failed!\n ");
                  return false;
             }
         }
     }
    return true;
}

bool CMsgPush::ExistChatPrivateMsg()
{
    return true;
}
bool CMsgPush::ExistChatGroupMsg()
{
    return true;
}

bool CMsgPush::DealContactAddMsg()
{



    return true;
}
bool CMsgPush::DealChatPrivateMsg()
{
    true;
}
bool CMsgPush::DealChatGroupMsg()
{
    return true;
}
