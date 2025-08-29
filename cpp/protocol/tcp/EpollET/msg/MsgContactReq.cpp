#include "IMsgBase.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"MsgManage.h"
#include "../usr/UserManage.h"
#include"MsgContactReq.h"
#include"../protobuf/ContactAdd.pb.h"
#include"../protobuf/chat.pb.h"

CMsgContactAdd::CMsgContactAdd()
{
    m_usBodyLen = 0;
    //m_uMsgType = ENU_MARS_MSG_CONTACT_REQ_A;

}

CMsgContactAdd::~CMsgContactAdd()
{

}
void CMsgContactAdd ::Deal(const USERDATAINFO& Data)
{

    if(!m_mySqlEncap)
        {
            m_mySqlEncap =  static_cast<EncapMysql::CEncapMysql*>(CPublicFun::GetInstance()->m_sqlCom.GetOneConn());
        }

    MarsContactAdd::ContactAdd  msgReq;
    msgReq.ParseFromArray(Data.usrPackage.usrData, ntohl(Data.usrPackage.nBodyLen));

   printf("\n[Recv]   [Msg:   Add Contact  client A----> server ]  ,  Socket:%d", Data.iSocket);
    printf("userida:%d, useridb:%d, msgType:%d, optionType:%d\n ",msgReq.userida(), msgReq.useridb(), msgReq.msgtype(), msgReq.optiontype());


     int iSocket = CUsrManage::GetInstance()->GetUsrSocket(msgReq.useridb());
     printf("CMsgContactAdd ::Deal() iSocket:%d\n", iSocket);

     int32_t errorcode = 0;
     int nRet = 0;

     //是否在线
     if(iSocket > 0)
     {
         nRet = TransformServerToB(msgReq, iSocket);
         if(nRet < 0)
         {
                errorcode = 13;
         }
         else
         {

         }

     }
     else // 离线
     {
            errorcode = 14;
     }



    MarsMsgChat::SendMsgRsp msgRsp;
    msgRsp.set_err_code(1000);

    uint32_t uTemp = 0;
    msgRsp.set_errmsg(uTemp);

    msgRsp.set_msgid(0);

    string strTemp = "reserve";
    msgRsp.set_access_token(strTemp);

    strTemp = "reserve";
    msgRsp.set_converstionid(strTemp);

    msgRsp.set_msguniqueid(0);

    printf("\n[Send ACK]   [Msg:   Add Contact:  Server--->Client A]  ,  Socket:%d", Data.iSocket);
     printf("\n errmsg:%d, msgid:%d, access_token:%s, converstionid:%s,msguniqueid:%d\n ",msgRsp.errmsg(), msgRsp.msgid(), msgRsp.access_token(), msgRsp.converstionid(), msgRsp.msguniqueid());


     msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
     m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSendData.iSocket = Data.iSocket;

    m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSendData.usrPackage.nSeq = Data.usrPackage.nSeq;

    uTemp = ENU_MARS_MSG_CONTACT_REQ_A;
    m_usrSendData.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSendData.usrPackage.nVersion = htonl(uTemp);

    // ack
    CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

     CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

    return ;
}

bool CMsgContactAdd::TransformServerToB(const  MarsContactAdd::ContactAdd&  contact, int iSocket)
{
    memset(m_szSql, 0, sizeof(m_szSql));

    snprintf(m_szSql, sizeof(m_szSql),"SELECT name,head,birthday,mail,token,sex,phone FROM tb_usrinfo WHERE uid = %d", contact.userida());

     int nRet = 0;
     if(m_mySqlEncap)
     {
         nRet = m_mySqlEncap->SelectQuery(m_szSql);
         {
             if(nRet  == 0)
             {
                 SendMsgToB(contact, iSocket);
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

bool CMsgContactAdd::SendMsgToB(const  MarsContactAdd::ContactAdd&  contact , int iSocket)
{
    vector<string>& vc = m_mySqlEncap->GetSelectResult();

    unsigned int  nMsgItem = vc.size() / m_mySqlEncap->m_iFields;


  MarsContactAdd::UserInfoAB msgRsp;

  uint32_t uTemp = ENU_CONTACT_REQUEST;	//消息推送
  msgRsp.set_msgtype(uTemp);

  //uTemp = 0;
  msgRsp.set_typeoption(contact.optiontype());

  msgRsp.set_userid(contact.userida());

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
      ++iter;

      printf("\n[Send]   [Msg:   Add Contact:  Server--->Client B]  ,  Socket:%d", iSocket);
       printf("msgtype:%d, typeoption:%d, userid:%d, name:%s,head:%d,birthday:%s, email:%s,token:%s, sex:%d,phone:%s\n ",
              msgRsp.msgtype(), msgRsp.typeoption(), msgRsp.userid(), msgRsp.name(), msgRsp.head(), msgRsp.birthday(), msgRsp.email(), msgRsp.token(), msgRsp.sex(), msgRsp.phone());


      msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
      m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSendData.iSocket = iSocket;

     m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
     m_usrSendData.usrPackage.nSeq = 0;

     uTemp = ENU_MARS_MSG_CONTACT_REQ_B;
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

 bool CMsgContactAdd::StoreContactReqToDB(const  MarsContactAdd::ContactAdd&  msg)
 {
     memset(m_szSql, 0, sizeof(m_szSql));
      int64_t uMsgID = CPublicFun::GetInstance()->GetSystemTime();


     snprintf(m_szSql, sizeof(m_szSql),"call call sp_user_addcontact(%d,%d, @ret )",  \
             msg.userida(),msg.useridb());

     return ExecuteMysql(m_szSql);
 }



