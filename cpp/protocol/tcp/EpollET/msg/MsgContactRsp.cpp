
#include"MsgContactRsp.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"MsgManage.h"
#include "../usr/UserManage.h"
#include"../protobuf/ContactAdd.pb.h"
#include"../protobuf/chat.pb.h"

CMsgContactRsp::CMsgContactRsp()
{
    m_usBodyLen = 0;
    //m_uMsgType = ENU_MARS_MSG_CONTACT_RSP;

}

CMsgContactRsp::~CMsgContactRsp()
{

}
void CMsgContactRsp ::Deal(const USERDATAINFO& Data)
{
    if(!m_mySqlEncap)
        {
            m_mySqlEncap =  static_cast<EncapMysql::CEncapMysql*>(CPublicFun::GetInstance()->m_sqlCom.GetOneConn());
        }

    MarsContactAdd::ContactAdd  msgReq;
    msgReq.ParseFromArray(Data.usrPackage.usrData, ntohl(Data.usrPackage.nBodyLen));

   printf("\n[Recv]   [Msg:   Add Contact  client B----> server ]  ,  Socket:%d", Data.iSocket);
     printf("userida:%d, useridb:%d, msgType:%d, optionType:%d\n ",msgReq.userida(), msgReq.useridb(), msgReq.msgtype(), msgReq.optiontype());

     int iSocket = CUsrManage::GetInstance()->GetUsrSocket(msgReq.userida());

     int32_t errorcode = 0;
     int nRet = 0;

     //是否在线
     if(iSocket > 0)
     {
         nRet = TransformServerToA(msgReq, iSocket);
         if(nRet < 0)
         {
                errorcode = 15;
         }
         else
         {

         }

     }
     else
     {
            errorcode = 16;
     }

#if 1

      MarsMsgChat:: SendMsgRsp msgRsp;


     if(errorcode == 0)
     {
         msgRsp.set_err_code(1000);
         msgRsp.set_errmsg(0);
     }
     else
     {
         msgRsp.set_err_code(1001);
         msgRsp.set_errmsg(errorcode);
     }

    uint32_t uTemp = 0;

    msgRsp.set_msgid(0);

    string strTemp = "reserve";
    msgRsp.set_access_token(strTemp);

    strTemp = "reserve";
    msgRsp.set_converstionid(strTemp);

     msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
     m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSendData.iSocket = Data.iSocket;

    m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSendData.usrPackage.nSeq = Data.usrPackage.nSeq;

    uTemp = ENU_MARS_MSG_CONTACT_RSP;
    m_usrSendData.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSendData.usrPackage.nVersion = htonl(uTemp);

    // ack
    CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

     CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

#endif
    return ;
}


 bool CMsgContactRsp::TransformServerToA(const MarsContactAdd::ContactAdd& contact, int iSocket)
 {
     memset(m_szSql, 0, sizeof(m_szSql));

     snprintf(m_szSql, sizeof(m_szSql),"SELECT name,head,birthday,mail,token,sex,phone FROM tb_usrinfo WHERE uid = %d", contact.useridb());

      int nRet = 0;
      if(m_mySqlEncap)
      {
          nRet = m_mySqlEncap->SelectQuery(m_szSql);
          {
              if(nRet  == 0)
              {
                  SendMsgToA(contact, iSocket);
              }
              else
              {
                   printf("GetUserinfo failed!\n ");
                   return false;
              }
          }
      }


     return true;
 }


 bool CMsgContactRsp::SendMsgToA( const  MarsContactAdd::ContactAdd&  contact, int iSocket)
 {
     vector<string>& vc = m_mySqlEncap->GetSelectResult();

     unsigned int  nMsgItem = vc.size() / m_mySqlEncap->m_iFields;


   MarsContactAdd::UserInfoAB msgRsp;

   uint32_t uTemp = ENU_CONTACT_RSP;	//消息推送
   msgRsp.set_msgtype(uTemp);

   msgRsp.set_typeoption(contact.optiontype());

   msgRsp.set_userid(contact.useridb());


     //vector对应字段:   tb_usrinfo(name,head,birthday,mail,token,sex,phone)

   vector<string>::iterator iter = vc.begin();


   //理论上只有一条数据，不需要循环，只为保持代码一
   for(unsigned int i = 0; i < nMsgItem; ++i)
   {
       msgRsp.set_name(iter->c_str());
       ++iter;
       msgRsp.set_head(iter->c_str());
       ++iter;
       msgRsp.set_birthday(iter->c_str());
       ++iter;
       msgRsp.set_email(iter->c_str());
       ++iter;

       msgRsp.set_token(iter->c_str());
       ++iter;

       msgRsp.set_sex(atoi(iter->c_str()));
       ++iter;
       msgRsp.set_phone(iter->c_str());

       ++iter;printf("\n[Send]   [Msg:   Add Contact:  Server--->Client A]  ,  Socket:%d", iSocket);
       printf("msgtype:%d, typeoption:%d, userid:%d, name:%s,head:%d,birthday:%s, email:%s,token:%s, sex:%d,phone:%s\n ",
              msgRsp.msgtype(), msgRsp.typeoption(), msgRsp.userid(), msgRsp.name(), msgRsp.head(), msgRsp.birthday(), msgRsp.email(), msgRsp.token(), msgRsp.sex(), msgRsp.phone());



       msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
       m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

      m_usrSendData.iSocket = iSocket;

      m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
      m_usrSendData.usrPackage.nSeq = 0;

      uTemp = ENUI_MARS_MSG_CONTACT_RSP_A;
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
