#include "MsgChatGroup.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"MsgManage.h"
#include"../usr/UserManage.h"
#include"../protobuf/chat.pb.h"

CMsgChatGroup::CMsgChatGroup()
{
    m_usBodyLen = 0;
    //m_uMsgType = ENU_MARS_MSG_CHAT_GROUP;
    memset(m_szSql, 0, sizeof(m_szSql));

}

CMsgChatGroup::~CMsgChatGroup()
{
    CPublicFun::GetInstance()->m_sqlCom.ReleaseOneConn(m_mySqlEncap);
}

void CMsgChatGroup::Deal(const USERDATAINFO& Data)
{
    if(!m_mySqlEncap)
    {
        m_mySqlEncap =  static_cast<EncapMysql::CEncapMysql*>(CPublicFun::GetInstance()->m_sqlCom.GetOneConn());
    }

    MarsMsgChat::SendMsgReq msgReq;
    msgReq.ParseFromArray(Data.usrPackage.usrData, ntohl(Data.usrPackage.nBodyLen));

    printf("\n[RECV    CHATGROUP  client ----> server\n   Socket:%d]\n", Data.iSocket);
    printf("n\naccess_token:%s, userFrom:%s,userTo:%s,text:%s,msgType:%d,headimage:%s,sendName:%s,senduserID:%s,conversation:%s,sendtype:%d\n\n\n",
           msgReq.access_token().c_str(),  msgReq.userfrom().c_str(), msgReq.userto().c_str(),msgReq.text().c_str(),msgReq.msgtype(),msgReq.headimage().c_str(),
           msgReq.sendname().c_str(),msgReq.senduserid().c_str(),msgReq.converstionid().c_str(), msgReq.sendtype());

    int errorCode = 0;

    SendAckToClientB(msgReq, Data, errorCode);

    if(!GetGroupMember(msgReq))
    {
        printf("CMsgChatGroup::Deal()  GetGroupMember() failed!\n  ");
        return ;
    }

    SendBoardcastMsg(msgReq, Data);

    return ;
}

 bool CMsgChatGroup::SendAckToClientB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data,  const int32_t errorCode)
 {
     MarsMsgChat::SendMsgRsp msgRsp;

     int32_t  uTemp = 0;
     if(errorCode == 0)
     {
         msgRsp.set_err_code(1000);
         msgRsp.set_errmsg(0);
     }
     else
     {
         msgRsp.set_err_code(1001);
         msgRsp.set_errmsg(errorCode);
     }

     //int64_t uTime = CPublicFun::GetInstance()->GetSystemTime();
    // msgRsp.set_msgid(uTime);
     msgRsp.set_msgid(CPublicFun::GetInstance()->GetSystemTime());


     string strTemp = "reserve";
     msgRsp.set_access_token(msg.access_token());

     msgRsp.set_converstionid(msg.converstionid());

     msgRsp.set_msguniqueid(msg.msguniqueid());


     printf("\n[Send    ACK]   [Msg:   CHAT GROUT  Server----> Client A \n ]  ,  Socket:%d\n", Data.iSocket);
     printf("err_code:%d, errmsg:%d,msgid:%d,access_token:%s,converstionid:%s,msguniqueid:%d \n",
            msgRsp.err_code(),  msgRsp.errmsg(), msgRsp.msgid(),msgRsp.access_token(),msgRsp.converstionid(),msgRsp.msguniqueid());


      msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
      m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSendData.iSocket = Data.iSocket;

     m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
     m_usrSendData.usrPackage.nSeq = Data.usrPackage.nSeq;

     if(m_uCmdId == ENU_MARS_MSG_CHAT_GROUP)
     {
         uTemp = ENU_MARS_MSG_CHAT_GROUP;
     }
     else if(m_uCmdId == ENU_MARS_MSG_SYSTEM_PUSH)
     {
         uTemp = ENU_MARS_MSG_SYSTEM_PUSH_B;
     }
     else
     {

     }
     //uTemp = ENU_MARS_MSG_CHAT_GROUP;
     m_usrSendData.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
     m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

     uTemp = 200;
     m_usrSendData.usrPackage.nVersion = htonl(uTemp);

     CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

      CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

     return true;
 }

bool CMsgChatGroup::SendBoardcastMsg(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data)
{
    set_intersection(m_mySqlEncap->m_setSelectResult.begin(), m_mySqlEncap->m_setSelectResult.end(), CUsrManage::GetInstance()->m_setUsrOnline.begin(), \
                     CUsrManage::GetInstance()->m_setUsrOnline.end(), inserter(m_setGroupOnLine, m_setGroupOnLine.begin()));



    MarsMsgChat::PushRsp rsp;

    int32_t uTemp = 0;
    if(m_uCmdId == ENU_MARS_MSG_CHAT_GROUP)
    {
        uTemp = 2;
    }
    else if(m_uCmdId == ENU_MARS_MSG_SYSTEM_PUSH)
    {
        uTemp = 1;
    }
    else
    {

    }
    rsp.set_msgpushtype(uTemp);

    uTemp = 0;
    rsp.set_errmsg(uTemp);

    uTemp = 0;
    rsp.set_state(uTemp);

    string strTemp = "00";
    rsp.set_accesstoken(msg.access_token());

    rsp.set_userfrom(msg.userfrom());

    rsp.set_userto(msg.userto());

    rsp.set_text(msg.text());

    rsp.set_headimage(msg.headimage());

    rsp.set_sendname(msg.sendname());

    rsp.set_senduserid(msg.senduserid());

    rsp.set_converstionid(msg.converstionid());

    rsp.set_sendtype(msg.sendtype());

   rsp.set_msgtype(msg.msgtype());

   rsp.set_msgid(CPublicFun::GetInstance()->GetSystemTime());

   printf("[Send ]   [Msg:   CHATGROUP  Server----> GROUP \n ]  \n");
   printf("msgpushtype:%d, errmsg:%d,state:%d,access_token:%s,userfrom:%s,userto:%s,text:%s,headimage:%s,sendname:%s,senduserid:%s,converstionid:%s,sendtype:%d,msgtype:%d,msgid:%d \n",
          rsp.msgpushtype(),  rsp.errmsg(), rsp.state(),rsp.accesstoken(),rsp.userfrom(),rsp.userto(),rsp.text(),rsp.headimage(),rsp.sendname(),rsp.senduserid(),
          rsp.converstionid(),rsp.sendtype(),rsp.msgtype(),rsp.msgid());

    rsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
    m_usrSendData.nMsgLen = rsp.ByteSize()  + MARS_MSG_HEAR_LEN;



   m_usrSendData.usrPackage.nBodyLen = htonl(rsp.ByteSize());

   uTemp = 0;
   m_usrSendData.usrPackage.nSeq = htonl(uTemp);

   if(m_uCmdId == ENU_MARS_MSG_CHAT_GROUP)
   {
       uTemp = ENU_MARS_MSG_CHAT_GROUP_ALL;
   }
   else if(m_uCmdId == ENU_MARS_MSG_SYSTEM_PUSH)
   {
       uTemp = ENU_MARS_MSG_SYSTEM_PUSH_B;
   }
   else
   {

   }
   m_usrSendData.usrPackage.nCmd = htonl(uTemp);

   uTemp = MARS_MSG_HEAR_LEN;
   m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

   uTemp = 200;
   m_usrSendData.usrPackage.nVersion = htonl(uTemp);




   set<uint32_t>::const_iterator iter;
   CMsgManage* pMsg = CMsgManage::GetInstance();
   CPublicFun* pFun = CPublicFun::GetInstance();
   CUsrManage* pUsr = CUsrManage::GetInstance();

   int iSocket = 0;

   for(iter = m_setGroupOnLine.begin(); iter != m_setGroupOnLine.end(); ++iter)
   {
       iSocket = pUsr->m_mapUsrLogin[*iter].iSocket;

       if(iSocket == Data.iSocket)
       {
           continue;
       }

       m_usrSendData.iSocket = iSocket;
       pMsg->MsgDequeSendPush(m_usrSendData);

        pFun->AwakenWriteEvent(m_usrSendData.iSocket);
          printf("[Send ]   [Msg:   CHATGROUP  Server----> GROUP \n ]  ,  Socket:%d\n", iSocket);
   }


   return true;
}


bool  CMsgChatGroup::GetGroupMember(const MarsMsgChat::SendMsgReq& msg)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT userid FROM tb_grouprelate WHERE groupId = %d",  atoi(msg.userto().c_str()));

    // printf("CSendMsgReq::GetConversationID(), szSql:%s\n\n",m_szSql);


     int nRet = 0;
     if(m_mySqlEncap)
     {
         nRet = m_mySqlEncap->SelectQuery(m_szSql, ENU_SELECTDATA_SET);
         {
             if(nRet  == 0)
             {
                 //筛选出在线用户

                 return true;
             }
             else
             {
                  printf("CMsgChatGroup::GetGroupMember( ) failed!,szSql:%s\n ",m_szSql);
                  return false;
             }
         }
     }






    return true;
}
