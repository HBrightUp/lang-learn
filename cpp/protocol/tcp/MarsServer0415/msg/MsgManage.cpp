#include<iostream>
#include<vector>
#include "MsgManage.h"
#include "MsgInfo.h"
#include"usr/UserManage.h"
#include <unistd.h>
#include <arpa/inet.h>
#include"epoll/EpollServer.h"

using namespace std;


CMsgManage::CMsgManage()
{
    m_pFun = CPublicFun::GetInstance();
    m_mySqlEncap = NULL;
}

CMsgManage::~CMsgManage()
{

    pthread_mutex_destroy(&m_mtxMsgDuqueSend);
}

bool CMsgManage::Init()
{
    if(!m_mySqlEncap)
        {
            m_mySqlEncap =  static_cast<EncapMysql::CEncapMysql*>(CPublicFun::GetInstance()->m_sqlCom.GetOneConn());
        }

    if(!m_mySqlEncapOffline)
        {
            m_mySqlEncapOffline =  static_cast<EncapMysql::CEncapMysql*>(CPublicFun::GetInstance()->m_sqlCom.GetOneConn());
        }

    m_pOfflineMsg  =  new STOFFLINEMSG;
    memset(m_szHead, 0, sizeof(m_szHead));
    memset(m_szName, 0, sizeof(m_szName));
    m_userIdLogin = 0;



     pthread_mutex_init(&m_mtxMsgDuqueSend, NULL);
    return true;
}

void CMsgManage::MsgDequeSendPush(USERDATAINFO& msgData)
{
    pthread_mutex_lock(&m_mtxMsgDuqueSend);
    m_msgDuqueSend.push_back(msgData);
    pthread_mutex_unlock(&m_mtxMsgDuqueSend);
}
void CMsgManage::MsgDequeSendPopFront()
{
    pthread_mutex_lock(&m_mtxMsgDuqueSend);
    m_msgDuqueSend.pop_front();
    pthread_mutex_unlock(&m_mtxMsgDuqueSend);
}

bool CMsgManage::IsvalidMsg(int fd, uint32_t cmd)
{
    //未登陆情况下只接收心跳消息与登陆消息
   if(CUsrManage::GetInstance()->GetUsrUserId(fd) == 0 &&
           cmd != ENU_MARS_MSG_LOGIN &&
           cmd != ENU_MARS_MSG_KEEPLIVE)
   {
       return false;
   }

    return true;
}

bool CMsgManage::Deal(const USERDATAINFO& msgInfo )
{
    //所有消息处理分类
    m_uCmdId = ntohl(msgInfo.usrPackage.nCmd);

    switch(m_uCmdId)
    {
     case ENU_MARS_MSG_CHAT_PRIVATE:
    {
        printf("\n\n[ RECV ] [ CHATPRIVATE]  fd:%d\n\n", msgInfo.fd);
        DealChatPrivate(msgInfo);
        break;
    }
    case ENU_MARS_MSG_KEEPLIVE:
    {
        printf("\n\n[ RECV ] [ KEEPLIVE]  fd:%d\n\n", msgInfo.fd);
        DealKeepLive(msgInfo);

        break;
    }
    case ENU_MARS_MSG_LOGIN:
    {
        printf("\n\n[ RECV ] [ LOGIN]  fd:%d\n\n", msgInfo.fd);
        DealLogin(msgInfo);
        break;
    }
    case ENU_MARS_MSG_LOGOUT:
    {
        printf("\n\n[ RECV ] [ LOGOUT]  fd:%d\n\n", msgInfo.fd);
        DealLogout(msgInfo);
        break;
    }

    case ENU_MARS_MSG_CONTACT_REQ_A:
    {
        printf("\n\n[ RECV ] [ CONTACTREQ ]  fd:%d\n\n", msgInfo.fd);
        DealAddContactReqByClientA(msgInfo);
        break;
    }

    case ENU_MARS_MSG_CONTACT_RSP_B:
    {
        printf("\n\n[ RECV ] [ CONTACTRSP ]  fd:%d\n\n", msgInfo.fd);
        DealAddContactRspByClientB(msgInfo);
        break;
    }

    case ENU_MARS_MSG_CHAT_GROUP_A:
    case ENU_MARS_MSG_SYSTEM_PUSH:
    {
        printf("\n\n[ RECV ] [ CHATGROUP]  fd:%d\n\n", msgInfo.fd);
          DealChatGroup(msgInfo);
        break;
    }
    case ENU_MARS_MSG_OFFLINE_B:
    {
        printf("\n\n[ RECV ] [ OFFLINE]  fd:%d\n\n", msgInfo.fd);
        DealOfflineByClientB(msgInfo);
        break;
    }

    default:
    {
        printf("[ WARNING ]CMsgManage::Deal() Unknown message,cmd:%d\n", m_uCmdId);
        break;
    }
    }

    return true;
}

bool CMsgManage::DealKeepLive(const USERDATAINFO& usrInfo)
{
    m_usrSend.fd = usrInfo.fd;
    m_usrSend.nMsgLen = MARS_MSG_HEAR_LEN;

    memcpy(&m_usrSend.usrPackage, &usrInfo.usrPackage, MARS_MSG_HEAR_LEN );

    //printf("[ KEEPLIVE RECV ]  fd:%d\n", usrInfo.fd);

    MsgDequeSendPush(m_usrSend);

    return true;
}

bool CMsgManage::DealAddContactReqByClientA(const USERDATAINFO& usrInfo)
{
    MarsMsgChat::ContactAdd  msgReq;
    bool bRet = false;
    string  strCoversationID;
    int32_t errorcode = 0;

    bRet = msgReq.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));
    if(bRet)
    {
        printf("userida:%d, useridb:%d, msgType:%d, optionType:%d\n ",msgReq.userida(), msgReq.useridb(), msgReq.msgtype(), msgReq.optiontype());

        //将信息加入数据表中tb_contacts
        if(StoreAddcontactToDB(msgReq))
        {
            if(GetConversationIDFromContactDB(msgReq.userida(), msgReq.useridb(), strCoversationID))
            {
                int fd = CUsrManage::GetInstance()->GetUsrSocket(msgReq.useridb());
                printf("[ INFO ]CMsgManage::DealAddContactReqByClientA() fd:%d\n", fd);

                //是否在线
                if(fd > 0)
                {
                    int nRet = TransformServerToB( fd, strCoversationID.data(),msgReq.optiontype(), msgReq.userida());
                    if(nRet < 0)
                    {
                           errorcode = ERROR_DB_INSERT_ADD_CONTACT_TRANSIT;
                    }
                }
                else // 离线
                {
                       if(!StroreAddContactOfflineToDBByClientA(msgReq, strCoversationID.data()))
                       {
                           errorcode = ERROR_DB_INSERT_USER_OFFLLINE;
                       }
                }
            }
            else
            {
                 errorcode = ERROR_DB_GET_CONTACT_CONVERSATION;
            }
        }
        else
        {
            errorcode = ERROR_DB_INSERT_ADD_CONTACT;
        }
    }
    else
    {
        errorcode = ERROR_PROTOBUF_PARSE_FAILURE;
    }

    MarsMsgChat::SendMsgRsp msgRsp;
    uint32_t uTemp = 0;

    if(errorcode  == 0)
    {
        uTemp = ENU_MSG_SUCCESS;
    }
    else
    {
        uTemp = ENU_MSG_FAILURE;
    }

    msgRsp.set_err_code(uTemp);

    msgRsp.set_errmsg(errorcode);

    string strTemp;

    CPublicFun::GetInstance()->GetSystemTime(strTemp);
    msgRsp.set_msgid(strTemp.data());

    strTemp = "0";
    msgRsp.set_access_token(strTemp);
    msgRsp.set_converstionid(strTemp);

    msgRsp.set_msguniqueid(0);

     printf("\n errmsg:%d, msgid:%s, access_token:%s, converstionid:%s,msguniqueid:%lld\n ",
            msgRsp.errmsg(), msgRsp.msgid().data(), msgRsp.access_token().data(), msgRsp.converstionid().data(), msgRsp.msguniqueid());

     msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
     m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSend.fd = usrInfo.fd;

    m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;

    uTemp = ENU_MARS_MSG_CONTACT_REQ_A;
    m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSend.usrPackage.nVersion = htonl(uTemp);

    MsgDequeSendPush(m_usrSend);

    return true;
}

 bool CMsgManage::StoreAddcontactToDB(const  MarsMsgChat::ContactAdd&  contact)
 {
     memset(m_szSql, 0, sizeof(m_szSql));

     snprintf(m_szSql, sizeof(m_szSql),"call sp_user_addcontact(%d,%d, @ret )",\
              contact.userida(), contact.useridb());

     return ExecuteMysql(m_szSql);
 }

 bool CMsgManage::StroreAddContactOfflineToDBByClientA(const  MarsMsgChat::ContactAdd& contact, const char* szCoversationID)
 {
     memset(m_szSql, 0, sizeof(m_szSql));

     string strTemp;

     CPublicFun::GetInstance()->GetSystemTime(strTemp);

     //数据库中表tb_msg中字段sendType定义为聊天类型:   1单聊 ; 2群聊 ;  3添加好友;   s4添加好友回执
     uint32_t uSendType = ENU_MSG_OFFLINE_ADD_CONTACT_REQ;

     snprintf(m_szSql, sizeof(m_szSql),"INSERT INTO tb_msg(msgId, fromUsrId, toUsrId, sendType,msgType,conversationId) VALUES('%s',%d,%d,%d,%d,'%s')",\
              strTemp.data(),contact.userida(), contact.useridb(), uSendType,contact.msgtype(), szCoversationID);

     printf("CMsgManage::StroreAddContactOfflineToDBByClientA()   m_szSql:%s",m_szSql);
     return ExecuteMysql(m_szSql);
 }

 bool CMsgManage::GetConversationIDFromContactDB(uint32_t userIdA, uint32_t userIdB, string& strCoversationID)
 {
     memset(m_szSql, 0, sizeof(m_szSql));
     snprintf(m_szSql, sizeof(m_szSql),"SELECT conversationID FROM tb_contacts AS c WHERE c.uid = %d AND c.cuid = %d AND status = 0" ,  userIdA,  userIdB);

      if(m_mySqlEncap)
      {
              if(m_mySqlEncap->SelectQuery(m_szSql)  == 0)
              {
                 if(m_mySqlEncap->GetSelectResult().size() != 0)
                 {
                    // printf("CMsgManage::GetConversationIDFromContactDB() strCoversationID:%s, vc:%s",strCoversationID.data(), m_mySqlEncap->GetSelectResult()[0].data());
                     strCoversationID = m_mySqlEncap->GetSelectResult()[0];
                 }
              }
              else
              {
                   printf("[ ERROR ]CMsgManage::GetConversationIDFromContactDB() sql:%s \n ", m_szSql);
                   return false;
              }
      }

     return true;
 }


bool CMsgManage::DealLogin(const USERDATAINFO& usrInfo)
{
        MarsMsgChat::LoginRequst  msgLogin;

        bool bRet = false;
        uint32_t errorCode = 0;
        bRet = msgLogin.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));
        if(bRet)
        {
            printf("username:%s, userid:%d\n ", msgLogin.username().data(), msgLogin.userid());
        }
        else
        {
            errorCode = ERROR_PROTOBUF_PARSE_FAILURE;
        }


        MarsMsgChat::SendMsgRsp msgRsp;

        if(bRet)
        {
            msgRsp.set_err_code(1000);
            msgRsp.set_errmsg(0);
        }
        else
        {
            msgRsp.set_err_code(1001);
            msgRsp.set_errmsg(errorCode);

        }

        uint32_t uTemp = 0;
        string strTemp;
        CPublicFun::GetInstance()->GetSystemTime(strTemp);
        msgRsp.set_msgid(strTemp.data());

        strTemp = "0";
        msgRsp.set_access_token(strTemp);

        strTemp = "0";
        msgRsp.set_converstionid(strTemp);

        printf("\n\n[  SEND ]    fd:%d \n", usrInfo.fd);
         printf("err_code:%d, errmsg:%d, msgid:%s, access_token:%s,converstionid:%s\n\n ", \
                 msgRsp.err_code(), msgRsp.errmsg(), msgRsp.msgid().data(), msgRsp.access_token().c_str(), msgRsp.converstionid().c_str());

         msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));

         m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

        m_usrSend.fd = usrInfo.fd;

        m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
        m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;

        uTemp = ENU_MARS_MSG_LOGIN;
        m_usrSend.usrPackage.nCmd = htonl(uTemp);

         uTemp = MARS_MSG_HEAR_LEN;
        m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

        uTemp = 200;
        m_usrSend.usrPackage.nVersion = htonl(uTemp);

        MsgDequeSendPush(m_usrSend);

         if(bRet)
         {
             STUSRLOGIN stUsr;
             stUsr.fd = usrInfo.fd;

             CUsrManage* pUsr = CUsrManage::GetInstance();

             int fd = pUsr->GetUsrSocket(msgLogin.userid());

             //用户已在线的情况下再登陆，需要关掉原来的连接，同一帐号只允许一个设备登陆
             if(fd > 1)
             {
                 SendDeviceLogout(fd);
                 CPublicFun::GetInstance()->StopConnect(fd);
             }

             CUsrManage::GetInstance()->AddUsr(msgLogin.userid(), stUsr);


             m_userIdLogin = msgLogin.userid();

             //处理离线消息
             DealOfflineMsg(msgLogin.userid());
         }
        return true;
}

bool CMsgManage::SendDeviceLogout(int fd)
{
    m_usrSend.nMsgLen =  MARS_MSG_HEAR_LEN;

   m_usrSend.fd = fd;

   m_usrSend.usrPackage.nBodyLen = 0;
   m_usrSend.usrPackage.nSeq = 0;

   uint32_t uTemp = ENU_MARS_MSG_LOGOUT_DEVICE_A;
   m_usrSend.usrPackage.nCmd = htonl(uTemp);

    uTemp = MARS_MSG_HEAR_LEN;
   m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

   uTemp = PROTOBUF_VERSION;
   m_usrSend.usrPackage.nVersion = htonl(uTemp);

   MsgDequeSendPush(m_usrSend);

    return true;
}

bool CMsgManage::DealLogout(const USERDATAINFO& usrInfo)
{

    MarsMsgChat::LogoutRequst  msgLogout;

    bool bRet = false;
    uint32_t errorCode = 0;
   bRet =  msgLogout.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));

   if(bRet)
   {
         printf("\n fd:%d,userid:%d\n", usrInfo.fd, msgLogout.userid());
   }
   else
   {
       errorCode = ERROR_PROTOBUF_PARSE_FAILURE;
   }


    MarsMsgChat::SendMsgRsp msgRsp;

    if(bRet)
    {
        msgRsp.set_err_code(1000);
        msgRsp.set_errmsg(0);
    }
    else
    {
        msgRsp.set_err_code(1001);
        msgRsp.set_errmsg(errorCode);
    }

    uint32_t uTemp = 0;
    string strTemp;
    CPublicFun::GetInstance()->GetSystemTime(strTemp);
    msgRsp.set_msgid(strTemp.data());

    strTemp = "0";
    msgRsp.set_access_token(strTemp);

    strTemp = "0";
    msgRsp.set_converstionid(strTemp);

    printf("\n\n[ SEND ]  LOGOUT    fd:%d\n", usrInfo.fd);
     printf("err_code:%d, errmsg:%d, msgid:%s, access_token:%s,converstionid:%s\n ", \
             msgRsp.err_code(), msgRsp.errmsg(), msgRsp.msgid().data(), msgRsp.access_token().c_str(), msgRsp.converstionid().c_str());

     msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));

     m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSend.fd = usrInfo.fd;

    m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;

    uTemp = ENU_MARS_MSG_LOGOUT;
    m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSend.usrPackage.nVersion = htonl(uTemp);

    MsgDequeSendPush(m_usrSend);

    if(bRet)
    {
        CUsrManage::GetInstance()->DelUsrByUsrID(msgLogout.userid());
        m_pFun->EpollCtlFd(m_pFun->m_nEpollFd,usrInfo.fd, ENU_FD_DEL);

    }

    return true;
}

bool CMsgManage::DealOfflineMsg(int32_t userId)
{
    DealOfflineMsgDetail(userId, ENU_MSG_OFFLINE_METHOD_BASE);
    DealOfflineMsgDetail(userId, ENU_MSG_OFFLINE_METHOD_GROUP_CHAT);
    return true;
}

bool CMsgManage::DealOfflineMsgDetail(int32_t userId, enOfflineDealMethod nType)
{

    if(nType == ENU_MSG_OFFLINE_METHOD_BASE)
    {
        if(!GetOfflienMsg(userId))
        {
            return false;
        }
    }
    else if(nType == ENU_MSG_OFFLINE_METHOD_GROUP_CHAT)
    {
        if(!GetGroupChatOfflienMsg(userId))
        {
                return false;
       }
    }
    else
    {
        return false;
    }


    if(!m_mySqlEncapOffline)
    {
        return false;
    }

    //结果集确定为10个字段
    if(m_mySqlEncapOffline->m_iFields != MSG_OFFLINE_DB_FIELD)
    {
        return false;
    }

    m_vcOfflineMsg = m_mySqlEncapOffline->GetSelectResult();

    if(m_vcOfflineMsg.size() == 0)
    {
        printf("[ INFO ] CMsgManage::DealOfflineMsgDetail() Empty data.  nType:%d \n", nType);
        return false;
    }

    //数据顺序: msgId, fromUsrId, toUsrId, text,msgType, sendType, conversationId,createTs,len,width
    vector<string>::iterator iter;



    for(iter = m_vcOfflineMsg.begin(); iter != m_vcOfflineMsg.end(); ++iter)
    {
        memset(m_pOfflineMsg, 0, sizeof(STOFFLINEMSG));

        memcpy(m_pOfflineMsg->msgId, iter->c_str(), sizeof(m_pOfflineMsg->msgId));

        ++iter;
        m_pOfflineMsg->fromUsrId = atoi(iter->c_str());

        ++iter;
        m_pOfflineMsg->toUsrId = atoi(iter->c_str());

        ++iter;
        memcpy(m_pOfflineMsg->text, iter->c_str(), iter->length());

        ++iter;
        m_pOfflineMsg->msgType = atoi(iter->c_str());

        ++iter;
        m_pOfflineMsg->sendType = atoi(iter->c_str());

        ++iter;
       // printf("CMsgManage::DealOfflineMsgDetail() conversationId:%s\n", iter->c_str());
        memcpy(m_pOfflineMsg->conversationId, iter->c_str(), sizeof(m_pOfflineMsg->conversationId));

        ++iter;
        memcpy(m_pOfflineMsg->createTs, iter->c_str(), sizeof(m_pOfflineMsg->createTs));

        ++iter;
        m_pOfflineMsg->len = atoi(iter->c_str());

        ++iter;
        m_pOfflineMsg->width = atoi(iter->c_str());

        if(IsNeedDealMsg(m_pOfflineMsg, userId, nType))
        {
             DealOfflineData(m_pOfflineMsg, userId);
        }
    }

    return true;
}

bool CMsgManage::DealOfflineByClientB(const USERDATAINFO& usrInfo)
{
    MarsMsgChat::MsgOffline  msg;
    bool bRet = false;
    uint32_t  errorCode = 0;

    bRet = msg.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));

    if(bRet)
    {
        printf("userfrom:%d, userto:%d,msgtype:%d, sendtype:%d,converstionid:%s \n ", \
               msg.userfrom(), msg.userto(), msg.msgtype(), msg.sendtype(), msg.converstionid().data() );
    }
    else
    {
        errorCode = ERROR_PROTOBUF_PARSE_FAILURE;
    }

    MarsMsgChat::SendMsgRsp msgRsp;

    if(bRet)
    {
        msgRsp.set_err_code(1000);
        msgRsp.set_errmsg(0);
    }
    else
    {
        msgRsp.set_err_code(1001);
        msgRsp.set_errmsg(errorCode);
    }




    uint32_t uTemp = 0;
    string strTemp;

    CPublicFun::GetInstance()->GetSystemTime(strTemp);
    msgRsp.set_msgid(strTemp.data());

    strTemp = "0";
    msgRsp.set_access_token(strTemp);

    msgRsp.set_converstionid(msg.converstionid().data());

    printf("[ OFFLINE Send ]    fd:%d\n", usrInfo.fd);
     printf(",err_code:%d, errmsg:%d, msgid:%s, access_token:%s,converstionid:%s\n ", \
             msgRsp.err_code(), msgRsp.errmsg(), msgRsp.msgid().data(), msgRsp.access_token().c_str(), msgRsp.converstionid().c_str());

     msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));

     m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSend.fd = usrInfo.fd;

    m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;

    uTemp = ENU_MARS_MSG_OFFLINE_B;
    m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSend.usrPackage.nVersion = htonl(uTemp);

    MsgDequeSendPush(m_usrSend);
    if(bRet)
    {
        if(msg.sendtype() != ENU_MSG_OFFLINE_GROUP_CHAT)
        {
            RemoveOfflineMsgInDB(msg);
        }
    }
    return true;
}

 bool CMsgManage::RemoveOfflineMsgInDB(const MarsMsgChat::MsgOffline&  msg)
 {
     memset(m_szSql, 0, sizeof(m_szSql));

     snprintf(m_szSql, sizeof(m_szSql),"DELETE FROM tb_msg  WHERE tb_msg.fromUsrId = %d AND tb_msg.toUsrId = %d  \
        AND tb_msg.msgType = %d AND tb_msg.conversationId = '%s' ", msg.userfrom(), msg. userto(), msg.msgtype(), msg.converstionid().data());

    printf("[ INFO ]CMsgManage::RemoveOfflineMsgInDB() m_szSql:%s  \n",m_szSql );
     return ExecuteMysql(m_szSql);
 }

bool CMsgManage::DealOfflineGroupChat(const PSTOFFLINEMSG pMsg,  const int  fd)
{
    MarsMsgChat::PushRsp rsp;

    if(!GetGroupInfo(pMsg))
    {
        return false;
    }

    rsp.set_sendname(m_szName);
     rsp.set_headimage(m_szHead);

    int32_t uTemp = 0;

    uTemp = ENU_MSG_PUSH_OFFLINE;
    rsp.set_msgpushtype(uTemp);

    uTemp = ENU_MSG_SUCCESS;
    rsp.set_errmsg(uTemp);

    uTemp = 0;
    rsp.set_state(uTemp);

    string strTemp = "0";

    //离线消息不使用此字段
    rsp.set_accesstoken(strTemp.data());

    stringstream stream;
    stream << pMsg->fromUsrId;
    rsp.set_userfrom(stream.str());

    stream.str("");
    stream << pMsg->toUsrId;
    rsp.set_userto(stream.str());
    rsp.set_text(pMsg->text);
    rsp.set_senduserid(strTemp.data());
    rsp.set_converstionid(pMsg->conversationId);
    rsp.set_sendtype(pMsg->sendType);
   rsp.set_msgtype(pMsg->msgType);
   rsp.set_len(pMsg->len);
   rsp.set_width(pMsg->width);

   CPublicFun::GetInstance()->GetSystemTime(strTemp);
   rsp.set_msgid(strTemp.data());

   printf("msgpushtype:%d, errmsg:%d,state:%d,access_token:%s,userfrom:%s,userto:%s,text:%s,headimage:%s,sendname:%s,senduserid:%s,converstionid:%s,\
          sendtype:%d,msgtype:%d,msgid:%s, len:%d, width:%d \n",
          rsp.msgpushtype(),  rsp.errmsg(), rsp.state(),rsp.accesstoken().data(),rsp.userfrom().data(),rsp.userto().data(),rsp.text().data(),rsp.headimage().data(),
          rsp.sendname().data(),rsp.senduserid().data(),rsp.converstionid().data(),rsp.sendtype(),rsp.msgtype(),rsp.msgid().data(), rsp.len(), rsp.width());

    rsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
    m_usrSend.nMsgLen = rsp.ByteSize()  + MARS_MSG_HEAR_LEN;

   m_usrSend.usrPackage.nBodyLen = htonl(rsp.ByteSize());

   uTemp = 0;
   m_usrSend.usrPackage.nSeq = htonl(uTemp);

    uTemp = ENU_MARS_MSG_CHAT_GROUP_ALL;
   m_usrSend.usrPackage.nCmd = htonl(uTemp);

   uTemp = MARS_MSG_HEAR_LEN;
   m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

   uTemp = 200;
   m_usrSend.usrPackage.nVersion = htonl(uTemp);

   m_usrSend.fd = fd;
   MsgDequeSendPush(m_usrSend);

    return true;
}

bool CMsgManage::GetGroupChatOfflienMsg(int32_t userId)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT m.msgId,m.fromUsrId,m.toUsrId,m.text,m.msgType,m.sendType,m.conversationId, m.createTs, m.len,m.width  \
             FROM tb_msg AS m WHERE  m.sendType = 1 AND  m.conversationId IN (SELECT conversationId FROM tb_grouprelate AS g WHERE g.userId = %d ) \
ORDER BY m.createTs " ,\
            userId);

    if(m_mySqlEncapOffline)
    {
        if(m_mySqlEncapOffline->SelectQuery(m_szSql))
        {
            printf("[ ERROR ]  Sql:%s\n ", m_szSql);
            return false;
        }
    }

    return true;
}

bool CMsgManage::IsNeedDealMsg(const PSTOFFLINEMSG pMsg, const uint32_t userId, enOfflineDealMethod nType)
{
    //屏蔽A上线给自己发送消息的问题
    if(pMsg->fromUsrId == userId  && (pMsg->sendType == ENU_MSG_OFFLINE_PRIVATE_CHAT ||
                                      pMsg->sendType == ENU_MSG_OFFLINE_ADD_CONTACT_RSP))
    {
        return false;
    }

    if(nType == ENU_MSG_OFFLINE_METHOD_GROUP_CHAT)
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf),"%d_%d",pMsg->fromUsrId, userId);
        string strTemp = buf;

        CPublicFun* pFun = CPublicFun::GetInstance();

          string strOld = pFun->m_mapMsgOffline[strTemp];
           string strNew = pMsg->createTs;
           if(strOld.length() > 0)
          {
              if(strcmp(pMsg->createTs, strOld.c_str()) < 0)
              {
                  return false;
              }

          }

       pthread_mutex_lock(&pFun->m_mtxOffline);
       pFun->m_mapMsgOffline[strTemp] = strNew;
       pthread_mutex_unlock(&pFun->m_mtxOffline);
    }

    return true;
}

bool CMsgManage::DealOfflineData(const PSTOFFLINEMSG pMsg, const uint32_t userId)
 {
#if 0
     printf("[OFFLINE MSG]    msgId:%s, fromUsrId:%d, toUsrId:%d, text:%s,msgType:%d, sendType:%d, conversationId:%s",\
            pMsg->msgId, pMsg->fromUsrId, pMsg->toUsrId, pMsg->text, pMsg->msgType, pMsg->sendType, pMsg->conversationId);
    #endif

     //如果是好友回执，需要查找的是好友发起端的fd
     int  nFd = userId;
     if(pMsg->sendType == ENU_MSG_OFFLINE_ADD_CONTACT_RSP )
     {
         nFd = pMsg->fromUsrId;
     }

     int fd = CUsrManage::GetInstance()->GetUsrSocket(nFd);
     if(fd <= 0)
     {
         printf("[ ERROR ] CMsgManage::DealOfflineData() fd:%d\n", fd);
         return false;
     }

     switch(pMsg->sendType)
     {
         case ENU_MSG_OFFLINE_PRIVATE_CHAT:
         {
             SendOfflinePrivateChat(pMsg, fd);
             break;
         }
         case ENU_MSG_OFFLINE_GROUP_CHAT:
         {
             DealOfflineGroupChat(pMsg, fd);
             break;
         }
          case ENU_MSG_OFFLINE_ADD_CONTACT_REQ:
         {
            DealOfflineAddContactReq(pMsg,fd);

              break;
         }
         case ENU_MSG_OFFLINE_ADD_CONTACT_RSP:
         {
             DealOfflineAddContactRsp(pMsg,fd);
               break;
         }
         default:
         {
             printf("unknown offlien msg!\n");
              break;
         }
     }

     return true;
 }

 bool CMsgManage::DealOfflineAddContactRsp(const PSTOFFLINEMSG pMsg, int fd)
 {
     return TransformServerToA(fd,pMsg->conversationId, pMsg->sendType, pMsg->toUsrId);
 }
 bool CMsgManage::GetGroupInfo(const PSTOFFLINEMSG pMsg)
 {
     static uint32_t GroupId = 0;

     //同一组多条消息只查询一次该用记的身份信息
     if(GroupId != pMsg->toUsrId)
     {
         if(!GetGroupInfoFromDB(pMsg->toUsrId, m_mySqlEncap))
         {
             return false;
         }

         if(m_mySqlEncap->GetSelectResult().size() == 0)
         {
             return false;
         }

         vector<string>::iterator iter = m_mySqlEncap->GetSelectResult().begin();

         memset(m_szName, 0, sizeof(m_szName));
         memcpy(m_szName, iter->c_str(), sizeof(m_szName) );
        // printf("CMsgManage::GetuserInfo() iterName:%s, m_szName:%s\n", iter->c_str(), m_szName);

         ++iter;

          memset(m_szHead, 0, sizeof(m_szHead));
          memcpy(m_szHead, iter->c_str(), sizeof(m_szHead) );
          //printf("CMsgManage::GetuserInfo() iterHead:%s, m_szHead:%s\n", iter->c_str(), m_szHead);

          GroupId = pMsg->fromUsrId;
     }



     return true;
}

  bool CMsgManage::GetuserInfo(const PSTOFFLINEMSG pMsg)
  {
      static uint32_t userId = 0;

      //同一用户多条消息只查询一次该用记的身份信息
      if(userId != pMsg->fromUsrId)
      {
          if(!GetuserInfoFromDB(pMsg->fromUsrId, m_mySqlEncap))
          {
              return false;
          }

          //数据集vector中的数据：name,head,birthday,mail,token,sex,phone
          if(m_mySqlEncap->GetSelectResult().size() != MSG_USRINFO_DB_FIELD)
          {
              return false;
          }

          if(m_mySqlEncap->GetSelectResult().size() == 0)
          {
              return false;
          }

          vector<string>::iterator iter = m_mySqlEncap->GetSelectResult().begin();

          memset(m_szName, 0, sizeof(m_szName));
          memcpy(m_szName, iter->c_str(), sizeof(m_szName) );
          //printf("CMsgManage::GetuserInfo() iterName:%s, m_szName:%s\n", iter->c_str(), m_szName);

          ++iter;

           memset(m_szHead, 0, sizeof(m_szHead));
           memcpy(m_szHead, iter->c_str(), sizeof(m_szHead) );
           //printf("CMsgManage::GetuserInfo() iterHead:%s, m_szHead:%s\n", iter->c_str(), m_szHead);

           userId = pMsg->fromUsrId;
      }

      return true;
  }

 bool CMsgManage::SendOfflinePrivateChat(const PSTOFFLINEMSG pMsg, int fd)
 {
      if(!m_mySqlEncap)
      {
          return false;
      }

     if(!GetuserInfo(pMsg))
     {
         return false;
     }

     MarsMsgChat::PushRsp rsp;
     stringstream stream;
     stream << m_szName;

     rsp.set_sendname(stream.str() );

     stream.str("");
     stream << m_szHead;
      rsp.set_headimage(stream.str());

     int32_t uTemp = ENU_MSG_PUSH_OFFLINE;
     rsp.set_msgpushtype(uTemp);

     uTemp = 0;
     rsp.set_errmsg(uTemp);

     uTemp = 0;
     rsp.set_state(uTemp);

     string strTemp = "0";
     rsp.set_accesstoken(strTemp.data());

    stream.str("");
     stream << pMsg->fromUsrId;
     rsp.set_userfrom(stream.str());

   stream.str("");
     stream << pMsg->toUsrId;
     rsp.set_userto(stream.str());

      rsp.set_text(pMsg->text);
      rsp.set_senduserid(strTemp.data());
      rsp.set_converstionid(pMsg->conversationId);

      uTemp = 2;
     rsp.set_sendtype(uTemp);

     rsp.set_msgtype(pMsg->msgType);

     CPublicFun::GetInstance()->GetSystemTime(strTemp);
    rsp.set_msgid(strTemp.data());

    rsp.set_len(pMsg->len);
    rsp.set_width(pMsg->width);

    printf("[ SEND][ CHATPRIVATE  ]   Server----> Client B,  fd:%d\n", fd);
    printf("msgpushtype:%d, errmsg:%d,state:%d,access_token:%s,userfrom:%s,userto:%s,text:%s,headimage:%s,sendname:%s,senduserid:%s,\
           converstionid:%s,sendtype:%d,msgtype:%d,msgid:%s, len:%d, width:%d \n",rsp.msgpushtype(),  rsp.errmsg(), rsp.state(),rsp.accesstoken().data(),
           rsp.userfrom().data(),rsp.userto().data(),rsp.text().data(),rsp.headimage().data(),rsp.sendname().data(),rsp.senduserid().data(),rsp.converstionid().data(),
           rsp.sendtype(),rsp.msgtype(),rsp.msgid().data(), rsp.len(), rsp.width());

     rsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
     m_usrSend.nMsgLen = rsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSend.fd = fd;
    m_usrSend.usrPackage.nBodyLen = htonl(rsp.ByteSize());

    uTemp = 0;
    m_usrSend.usrPackage.nSeq = htonl(uTemp);

    uTemp = ENU_MARS_MSG_CHAT_PRIVET_B;
    m_usrSend.usrPackage.nCmd = htonl(uTemp);

    uTemp = MARS_MSG_HEAR_LEN;
    m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSend.usrPackage.nVersion = htonl(uTemp);

     MsgDequeSendPush(m_usrSend);

     return true;
 }
  bool CMsgManage::GetGroupInfoFromDB(int32_t userId,   EncapMysql::CEncapMysql* pMysql)
  {
      memset(m_szSql, 0, sizeof(m_szSql));
      snprintf(m_szSql, sizeof(m_szSql),"SELECT name, headImg, peopleNum FROM tb_group WHERE id = %d", userId);

       if(pMysql)
       {
           if(pMysql->SelectQuery(m_szSql))
           {
                printf("[ ERROR ] sql:%s\n ", m_szSql);
                return false;
           }
       }

      return true;
  }

 bool CMsgManage::GetuserInfoFromDB(int32_t userId,   EncapMysql::CEncapMysql* pMysql)
 {
     memset(m_szSql, 0, sizeof(m_szSql));
     snprintf(m_szSql, sizeof(m_szSql),"SELECT name,head,birthday,mail,token,sex,phone FROM tb_usrinfo WHERE uid = %d", userId);

      if(pMysql)
      {
          if(pMysql->SelectQuery(m_szSql))
          {
               printf("[ ERROR ] sql:%s\n ", m_szSql);
               return false;
          }
      }

     return true;
 }

bool CMsgManage::DealOfflineAddContactReq(const PSTOFFLINEMSG pMsg, int fd)
{
    return TransformServerToB(fd,pMsg->conversationId, pMsg->sendType, pMsg->fromUsrId);
}

bool CMsgManage::GetOfflienMsg(int32_t userId)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT msgId, fromUsrId, toUsrId, text,msgType, sendType, conversationId,createTs,len ,width FROM tb_msg WHERE \
             fromUsrId = %d OR toUsrId = %d" ,userId,  userId);

    if(m_mySqlEncapOffline)
    {
            if(m_mySqlEncapOffline->SelectQuery(m_szSql))
            {
                  printf("[ ERROR ]    sql:%s\n ", m_szSql);
                   return false;
            }
    }

    return true;
}
bool CMsgManage::TransformServerToB(int fd, const char* strCoversationID, uint32_t optionType, int32_t userIda)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT name,head,birthday,mail,token,sex,phone FROM tb_usrinfo WHERE uid = %d", userIda);

     int nRet = 0;
     if(m_mySqlEncap)
     {
         nRet = m_mySqlEncap->SelectQuery(m_szSql);
         {
             if(nRet  == 0)
             {
                 SendMsgToB(fd, strCoversationID,optionType, userIda);
             }
             else
             {
                  printf("[ ERROR ] sql:%s\n ", m_szSql);
                  return false;
             }
         }
     }

    return true;
}

bool CMsgManage::SendMsgToB(int fd, const char* strCoversationID, uint32_t optionType, int32_t userIda)
{
    vector<string>& vc = m_mySqlEncap->GetSelectResult();
    if(vc.size() == 0)
    {
        return false;
    }

    MarsMsgChat::UserInfoAB msgRsp;

    uint32_t uTemp = ENU_CONTACT_REQUEST;	//消息推送
    msgRsp.set_msgtype(uTemp);

    msgRsp.set_typeoption(optionType);

    msgRsp.set_userid(userIda);

    msgRsp.set_converstionid(strCoversationID);

    //vector对应字段:   tb_usrinfo(name,head,birthday,mail,token,sex,phone)

  vector<string>::iterator iter = vc.begin();


  //理论上只有一条数据

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

      printf("[ \n\n SEND ][ ADDCONTACT ]     Server--->Client B  ,  fd:%d\n", fd);
       printf("msgtype:%d, typeoption:%d, userid:%d, name:%s,head:%s,birthday:%s, email:%s,token:%s, sex:%d,phone:%s,converstionid:%s\n ",
              msgRsp.msgtype(), msgRsp.typeoption(), msgRsp.userid(), msgRsp.name().data(), msgRsp.head().data(), msgRsp.birthday().data(),
              msgRsp.email().data(), msgRsp.token().data(), msgRsp.sex(), msgRsp.phone().data(), msgRsp.converstionid().data());

      msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
      m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSend.fd = fd;

     m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());

     uTemp = 0;
     m_usrSend.usrPackage.nSeq = htonl(uTemp);

     uTemp = ENU_MARS_MSG_CONTACT_REQ_B;
     m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
     m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

     uTemp = 200;
     m_usrSend.usrPackage.nVersion = htonl(uTemp);

     MsgDequeSendPush(m_usrSend);

    return true;
}

bool CMsgManage::DealAddContactRspByClientB(const USERDATAINFO& usrInfo)
{;
    MarsMsgChat::ContactAdd  msgReq;
    string  strCoversationID;
    int fd = 0;
    uint32_t errorcode = 0;
    bool bRet = false;


    bRet = msgReq.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));

    if(bRet)
    {
        printf("userida:%d, useridb:%d, msgType:%d, optionType:%d\n ",msgReq.userida(), msgReq.useridb(), msgReq.msgtype(), msgReq.optiontype());

        if(GetConversationIDFromContactDB(msgReq.userida(), msgReq.useridb(), strCoversationID))
        {
            if(StoreResultToDB(msgReq))
             {
                fd = CUsrManage::GetInstance()->GetUsrSocket(msgReq.userida());

                //是否在线
                if(fd > 0)
                {
                    bRet = TransformServerToA( fd, strCoversationID.data(),msgReq.optiontype(), msgReq.useridb());
                    if(bRet)
                    {
                           errorcode = ERROR_ADDCONTACT_RSP;
                    }
                }
                else
                {
                       errorcode = ERROR_DB_INSERT_USER_OFFLLINE;
                }
             }
            else
            {
                errorcode = ERROR_DB_UPDATE_ADD_CONTACT;
            }
        }
    }
    else
    {
        errorcode = ERROR_PROTOBUF_PARSE_FAILURE;
    }


      MarsMsgChat:: SendMsgRsp msgRsp;


     if(errorcode == 0 || errorcode == ERROR_DB_INSERT_USER_OFFLLINE)
     {
         msgRsp.set_err_code(1000);
         msgRsp.set_errmsg(0);
     }
     else
     {
         msgRsp.set_err_code(1001);
         msgRsp.set_errmsg(errorcode);
     }

     string strTemp;
    CPublicFun::GetInstance()->GetSystemTime(strTemp);

    msgRsp.set_msgid(strTemp.data());

    strTemp = "0";
    msgRsp.set_access_token(strTemp);

    strTemp = "0";
    msgRsp.set_converstionid(strTemp);

    printf("\n\n[ SEND ACK ][ ADDCONTACT ]   Server--->Client B,  fd:%d", usrInfo.fd);
     printf("errmsg:%d, msgid:%s, access_token:%s, converstionid:%s,msguniqueid:%d\n ",
            msgRsp.errmsg(), msgRsp.msgid().data(), msgRsp.access_token().data(), msgRsp.converstionid().data(), msgRsp.msguniqueid());

     msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
     m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSend.fd = usrInfo.fd;

    m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;

    uint32_t uTemp = ENU_MARS_MSG_CONTACT_RSP_B;
    m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = PROTOBUF_VERSION;
    m_usrSend.usrPackage.nVersion = htonl(uTemp);

     MsgDequeSendPush(m_usrSend);

    return true;
}

bool CMsgManage::TransformServerToA(int fd, const char* strCoversationID,int32_t optionType,  int32_t userIdb)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT name,head,birthday,mail,token,sex,phone FROM tb_usrinfo WHERE uid = %d", userIdb);

     int nRet = 0;
     if(m_mySqlEncap)
     {
         nRet = m_mySqlEncap->SelectQuery(m_szSql);
         {
             if(nRet  == 0)
             {
                 SendMsgToA( fd, strCoversationID, optionType, userIdb);
             }
             else
             {
                  printf("[ ERROR ]  sql:%s\n ", m_szSql);
                  return false;
             }
         }
     }

    return true;
}

bool CMsgManage::StoreResultToDB( const  MarsMsgChat::ContactAdd&  contact)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"UPDATE tb_contacts SET status = %d WHERE uid = %d AND cuid = %d", \
             contact.optiontype(), contact.userida(), contact.useridb());

    if(!ExecuteMysql(m_szSql))
        return false;


    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"UPDATE tb_contacts SET status = %d WHERE uid = %d AND cuid = %d", \
             contact.optiontype(), contact.useridb(), contact.userida());

      return ExecuteMysql(m_szSql);
}

bool CMsgManage::StroreAddContactToDBByClientB(const  MarsMsgChat::ContactAdd& contact)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    string strTemp;

    CPublicFun::GetInstance()->GetSystemTime(strTemp);

    snprintf(m_szSql, sizeof(m_szSql),"INSERT INTO tb_msg(msgId,fromUsrId, toUsrId, sendType,msgType) VALUES(%s,%d,%d,%d,%d)",\
             strTemp.data(),contact.userida(), contact.useridb(), contact.msgtype(),contact.optiontype());

    return ExecuteMysql(m_szSql);

    return true;
}



bool CMsgManage::SendMsgToA( int fd, const char* strCoversationID, int32_t optionType, int32_t userIdb)
{
     printf("CMsgManage::SendMsgToA() strCoversationID:%s \n",strCoversationID);

    vector<string>& vc = m_mySqlEncap->GetSelectResult();

    if(vc.size() == 0)
    {
        return false;
    }

  MarsMsgChat::UserInfoAB msgRsp;

  uint32_t uTemp = ENU_CONTACT_RSP;	//消息推送
  msgRsp.set_msgtype(uTemp);

  msgRsp.set_typeoption(optionType);

  msgRsp.set_userid(userIdb);

  msgRsp.set_converstionid(strCoversationID);

  printf("CMsgManage::SendMsgToA() msgRsp.converstionid:%s \n",msgRsp.converstionid().data());

  vector<string>::iterator iter = vc.begin();

  do
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
      printf("[ADDCONTACT Send]    Server--->Client A  ,  fd:%d\n", fd);
      printf("msgtype:%d, typeoption:%d, userid:%d, name:%s,head:%s,birthday:%s, email:%s,token:%s, sex:%d,phone:%s, strCoversationID:%s \n ",
             msgRsp.msgtype(), msgRsp.typeoption(), msgRsp.userid(), msgRsp.name().data(), msgRsp.head().data(), msgRsp.birthday().data(),
             msgRsp.email().data(), msgRsp.token().data(), msgRsp.sex(), msgRsp.phone().data(), strCoversationID);



      msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
      m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSend.fd = fd;

     m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
     m_usrSend.usrPackage.nSeq = 0;

     uTemp = ENUI_MARS_MSG_CONTACT_RSP_A;
     m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
     m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

     uTemp = 200;
     m_usrSend.usrPackage.nVersion = htonl(uTemp);

     MsgDequeSendPush(m_usrSend);


  }while(0);     //   只读取第一条数据，正确情况下只有唯一一条数据，避免异常循环

    return true;
}

 bool CMsgManage::DealChatPrivate(const USERDATAINFO& usrInfo)
 {
     MarsMsgChat::SendMsgReq msgReq;

     bool bRet = false;
     //返回给客户端的错误码
     uint32_t  errorCode = 0;

     bRet = msgReq.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));
     if(bRet)
     {
         printf("[CHATPRIVATE Recv]   client A----> server  fd:%d\n", usrInfo.fd);
         printf("naccess_token:%s, userFrom:%s,userTo:%s,text:%s,msgType:%d,headimage:%s,sendName:%s,senduserID:%s,\
conversation:%s,sendtype:%d,len:%d,width:%d \n",msgReq.access_token().c_str(),  msgReq.userfrom().c_str(),
                msgReq.userto().c_str(),msgReq.text().c_str(),msgReq.msgtype(),msgReq.headimage().c_str(),msgReq.sendname().c_str(),
                msgReq.senduserid().c_str(),msgReq.converstionid().c_str(), msgReq.sendtype(), msgReq.len(), msgReq.width());

         //会话信息存入数据库
        if(StoreConversationID(msgReq))
         {
             //是否在线
             int fd = CUsrManage::GetInstance()->GetUsrSocket(atoi(msgReq.userto().c_str()));
             if(fd > 0)
             {
                 //转发消息给B,B收到信息后会有消息回执，这里不做失败处理
                 TransmitMsgToB(msgReq, usrInfo, fd);
             }
             else
             {
                 errorCode = ERROR_USER_OFFLINE;				//B不在线
                 //将消息记录存入到数据库
                 if(!StoreMsgToDB(msgReq))
                 {
                     errorCode = ERROR_DB_INSERT_MSG_FAILURE;		//存储离线消息记录失败
                 }
             }
         }
         else
         {
             errorCode = ERROR_DB_INSERT_CONVERSATION_FAILURE;		//存储会话信息失败
         }
     }
     else
     {
         errorCode =  ERROR_PROTOBUF_PARSE_FAILURE;
     }



     AckToA(msgReq, usrInfo, errorCode);

     return true;
 }


 bool CMsgManage::AckToA(const MarsMsgChat::SendMsgReq& msgReq, const USERDATAINFO& usrInfo,  const int32_t errorCode)
 {
         MarsMsgChat::SendMsgRsp msgRsp;

         int32_t  uTemp = 0;
         if(errorCode == 0 || errorCode == ERROR_USER_OFFLINE)
         {
             msgRsp.set_err_code(1000);
             msgRsp.set_errmsg(0);
         }
         else
         {
             msgRsp.set_err_code(1001);
             msgRsp.set_errmsg(errorCode);
         }

          string strTemp;
        CPublicFun::GetInstance()->GetSystemTime(strTemp);
        msgRsp.set_msgid(strTemp.data());

         strTemp = "0";
         msgRsp.set_access_token(msgReq.access_token());


         if(!GetConversationID(msgReq,  strTemp))
         {
             strTemp = "0";
         }
         msgRsp.set_converstionid(strTemp);

         msgRsp.set_msguniqueid(msgReq.msguniqueid());


         printf("[ SEND ] [ CHATPRIVATE Send    ACK]     Server----> ClientA \n ]  ,  fd:%d\n", usrInfo.fd);
         printf("err_code:%d, errmsg:%d,msgid:%s,access_token:%s,converstionid:%s,msguniqueid:%d \n",
                msgRsp.err_code(),  msgRsp.errmsg(), msgRsp.msgid().data(),msgRsp.access_token().data(),msgRsp.converstionid().data(),msgRsp.msguniqueid());


          msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
          m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

         m_usrSend.fd = usrInfo.fd;

         m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
         m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;

         uTemp = ENU_MARS_MSG_CHAT_PRIVATE;
         m_usrSend.usrPackage.nCmd = htonl(uTemp);

         uTemp = MARS_MSG_HEAR_LEN;
         m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

         uTemp = PROTOBUF_VERSION;
         m_usrSend.usrPackage.nVersion = htonl(uTemp);

         MsgDequeSendPush(m_usrSend);

     return true;
 }

  bool CMsgManage::TransmitMsgToB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& usrInfo, const uint32_t fd )
  {
      MarsMsgChat::PushRsp rsp;

      int32_t uTemp = ENU_MSG_PUSH_USER;
      rsp.set_msgpushtype(uTemp);

      uTemp = 0;
      rsp.set_errmsg(uTemp);

      uTemp = 0;
      rsp.set_state(uTemp);

      string strTemp = "0";
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

      CPublicFun::GetInstance()->GetSystemTime(strTemp);
     rsp.set_msgid(strTemp.data());

     rsp.set_len(msg.len());

     rsp.set_width(msg.width());

     printf("[ SEND ] [ CHATPRIVATE ]   Server----> Client B,  fd:%d\n", fd);
     printf("msgpushtype:%d, errmsg:%d,state:%d,access_token:%s,userfrom:%s,userto:%s,text:%s,headimage:%s,sendname:%s,senduserid:%s,\
            converstionid:%s,sendtype:%d,msgtype:%d,msgid:%s, len:%d, width:%d \n",
            rsp.msgpushtype(),  rsp.errmsg(), rsp.state(),rsp.accesstoken().data(),rsp.userfrom().data(),rsp.userto().data(),rsp.text().data(),
            rsp.headimage().data(),rsp.sendname().data(),rsp.senduserid().data(),
            rsp.converstionid().data(),rsp.sendtype(),rsp.msgtype(),rsp.msgid().data(), rsp.len(), rsp.width());

      rsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
      m_usrSend.nMsgLen = rsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSend.fd = fd;

     m_usrSend.usrPackage.nBodyLen = htonl(rsp.ByteSize());

     uTemp = 0;
     m_usrSend.usrPackage.nSeq = htonl(uTemp);

     uTemp = ENU_MARS_MSG_CHAT_PRIVET_B;
     m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
     m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

     uTemp = 200;
     m_usrSend.usrPackage.nVersion = htonl(uTemp);

      MsgDequeSendPush(m_usrSend);

      return true;
  }


 bool CMsgManage::StoreConversationID(const MarsMsgChat::SendMsgReq& msg)
 {
     memset(m_szSql, 0, sizeof(m_szSql));

     snprintf(m_szSql, sizeof(m_szSql),"call sp_conversation_save(%d,%d,'%s','%s',%d,%d, @ret )",\
              atoi(msg.userfrom().data()), atoi(msg.userto().data()), msg.converstionid().data(),  msg.text().data(),  msg.msgtype(), msg.sendtype());

     return ExecuteMysql(m_szSql);
 }



 bool CMsgManage::StoreMsgToDB(const MarsMsgChat::SendMsgReq& msg)
 {
     memset(m_szSql, 0, sizeof(m_szSql));
     string strTemp;
      CPublicFun::GetInstance()->GetSystemTime(strTemp);


     snprintf(m_szSql, sizeof(m_szSql),"INSERT INTO tb_msg(msgid,fromUsrid,toUsrid,text,msgType,sendType,conversationId,len,width) VALUES(%s, %d, %d,'%s',%d,%d,'%s',%d,%d)",  \
              strTemp.data(), atoi(msg.userfrom().c_str()),atoi( msg.userto().c_str()), msg.text().c_str(), msg.msgtype(), msg.sendtype(), msg.converstionid().c_str(), msg.len(),msg.width());

     return ExecuteMysql(m_szSql);
 }

 bool CMsgManage::GetConversationID(const MarsMsgChat::SendMsgReq& msg, string& strCoversationID )
 {
     memset(m_szSql, 0, sizeof(m_szSql));
     snprintf(m_szSql, sizeof(m_szSql),"SELECT conversationID FROM tb_contacts AS c WHERE c.uid = %s AND c.cuid = %s",  msg.userfrom().data(), msg.userto().c_str());

      int nRet = 0;
      if(m_mySqlEncap)
      {
          nRet = m_mySqlEncap->SelectQuery(m_szSql);
          {
              if(nRet  == 0)
              {
                 vector<string>& vc = m_mySqlEncap->GetSelectResult();
                 for(size_t  i = 0; i < vc.size(); ++i)
                 {
                     strCoversationID = vc[i];
                 }
              }
              else
              {
                   return false;
              }
          }
      }

      return true;
 }

 bool CMsgManage::ExecuteMysql(const char* szSql)
 {
     int nRet = 0;
    if(m_mySqlEncap)
    {
        nRet = m_mySqlEncap->ModifyQuery(szSql);
        if(nRet != 0)
        {
            printf("[ ERROR ] nRet:%d,  sql:%s\n ",nRet, szSql);
            return false;
        }
    }

     return true;
 }

 bool CMsgManage::DealChatGroup(const USERDATAINFO& usrInfo)
 {
     MarsMsgChat::SendMsgReq msgReq;
     bool bRet = false;
      uint32_t errorCode = 0;
     bRet = msgReq.ParseFromArray(usrInfo.usrPackage.usrData, ntohl(usrInfo.usrPackage.nBodyLen));

     if(bRet)
     {
         printf("naccess_token:%s, userFrom:%s,userTo:%s,text:%s,msgType:%d,headimage:%s,sendName:%s,senduserID:%s,conversation:%s,sendtype:%d, \
                len:%d, width:%d\n",
                msgReq.access_token().c_str(),  msgReq.userfrom().c_str(), msgReq.userto().c_str(),msgReq.text().c_str(),msgReq.msgtype(),msgReq.headimage().c_str(),
                msgReq.sendname().c_str(),msgReq.senduserid().c_str(),msgReq.converstionid().c_str(), msgReq.sendtype(), msgReq.len(), msgReq.width());

      if(StoreConversationID(msgReq))
      {
          if(GetGroupMember(msgReq))
          {
              SendBoardcastMsg(msgReq, usrInfo);

              if(!StoreMsgToDB(msgReq))
              {
                  errorCode =ERROR_DB_INSERT_MSG_FAILURE;
              }
          }
          else
          {
              errorCode =ERROR_DB_GET_GROUP_MEMBER;
          }
      }
      else
      {
          errorCode =ERROR_DB_INSERT_CONVERSATION_FAILURE;
      }
     }
     else
     {
         errorCode =ERROR_PROTOBUF_PARSE_FAILURE;
     }

     SendAckToClientB(msgReq, usrInfo, errorCode);

     return true;
 }

 bool CMsgManage::SendAckToClientB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& usrInfo,  const int32_t errorCode)
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


      string strTemp;
     CPublicFun::GetInstance()->GetSystemTime(strTemp);
     msgRsp.set_msgid(strTemp.data());

     strTemp = "0";
     msgRsp.set_access_token(msg.access_token());

     msgRsp.set_converstionid(msg.converstionid());

     msgRsp.set_msguniqueid(msg.msguniqueid());


     printf("[ SEND ] [ CHATGROUP    ACK]    Server----> ClientA ,  Socket:%d\n", usrInfo.fd);
     printf("err_code:%d, errmsg:%d,msgid:%s,access_token:%s,converstionid:%s,msguniqueid:%d \n",
            msgRsp.err_code(),  msgRsp.errmsg(), msgRsp.msgid().data(),msgRsp.access_token().data(),msgRsp.converstionid().data(),msgRsp.msguniqueid());


      msgRsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
      m_usrSend.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

     m_usrSend.fd = usrInfo.fd;

     m_usrSend.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
     m_usrSend.usrPackage.nSeq = usrInfo.usrPackage.nSeq;


     if(m_uCmdId == ENU_MARS_MSG_CHAT_GROUP_A)
     {
           uTemp = ENU_MARS_MSG_CHAT_GROUP_A;
     }
     else if(m_uCmdId == ENU_MARS_MSG_SYSTEM_PUSH)
     {
          uTemp = ENU_MARS_MSG_SYSTEM_PUSH;
     }
     else
     {

     }

     m_usrSend.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
     m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

     uTemp = 200;
     m_usrSend.usrPackage.nVersion = htonl(uTemp);

     MsgDequeSendPush(m_usrSend);

     return true;
 }

bool CMsgManage::SendBoardcastMsg(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& usrInfo)
{
    set_intersection(m_mySqlEncap->m_setSelectResult.begin(), m_mySqlEncap->m_setSelectResult.end(), CUsrManage::GetInstance()->m_setUsrOnline.begin(), \
                     CUsrManage::GetInstance()->m_setUsrOnline.end(), inserter(m_setGroupOnLine, m_setGroupOnLine.begin()));



    MarsMsgChat::PushRsp rsp;

    int32_t uTemp = 0;

    if(m_uCmdId == ENU_MARS_MSG_CHAT_GROUP_A)
    {
          uTemp = ENU_MSG_PUSH_USER;
    }
    else if(m_uCmdId == ENU_MARS_MSG_SYSTEM_PUSH)
    {
         uTemp = ENU_MSG_PUSH_SYSTEM;
    }
    else
    {

    }
    rsp.set_msgpushtype(uTemp);

    uTemp = 1000;
    rsp.set_errmsg(uTemp);

    uTemp = 0;
    rsp.set_state(uTemp);

    string strTemp = "00";
    rsp.set_accesstoken(msg.access_token());

    rsp.set_userfrom(msg.userfrom());

    rsp.set_userto(msg.userto());

    rsp.set_text(msg.text());

    rsp.set_headimage(msg.headimage());



    //qestion
    rsp.set_sendname(msg.sendname());

    rsp.set_senduserid(msg.senduserid());

    rsp.set_converstionid(msg.converstionid());

    rsp.set_sendtype(msg.sendtype());

   rsp.set_msgtype(msg.msgtype());

   rsp.set_len(msg.len());
   rsp.set_width(msg.width());

   CPublicFun::GetInstance()->GetSystemTime(strTemp);
   rsp.set_msgid(strTemp.data());

   printf("[ SEND  CHATGROUP  Server----> GROUP \n ");
   printf("msgpushtype:%d, errmsg:%d,state:%d,access_token:%s,userfrom:%s,userto:%s,text:%s,headimage:%s,sendname:%s,senduserid:%s,converstionid:%s,\
          sendtype:%d,msgtype:%d,msgid:%s, len:%d, width:%d \n",
          rsp.msgpushtype(),  rsp.errmsg(), rsp.state(),rsp.accesstoken().data(),rsp.userfrom().data(),rsp.userto().data(),rsp.text().data(),rsp.headimage().data(),rsp.sendname().data(),rsp.senduserid().data(),
          rsp.converstionid(),rsp.sendtype(),rsp.msgtype(),rsp.msgid().data(), rsp.len(), rsp.width());

    rsp.SerializeToArray(m_usrSend.usrPackage.usrData, sizeof(m_usrSend.usrPackage.usrData));
    m_usrSend.nMsgLen = rsp.ByteSize()  + MARS_MSG_HEAR_LEN;



   m_usrSend.usrPackage.nBodyLen = htonl(rsp.ByteSize());

   uTemp = 0;
   m_usrSend.usrPackage.nSeq = htonl(uTemp);

    if(m_uCmdId == ENU_MARS_MSG_CHAT_GROUP_A)
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

   m_usrSend.usrPackage.nCmd = htonl(uTemp);

   uTemp = MARS_MSG_HEAR_LEN;
   m_usrSend.usrPackage.nHeadLen = htonl(uTemp);

   uTemp = 200;
   m_usrSend.usrPackage.nVersion = htonl(uTemp);

   set<uint32_t>::const_iterator iter;
   CPublicFun* pFun = CPublicFun::GetInstance();
   CUsrManage* pUsr = CUsrManage::GetInstance();

   int fd = 0;

   for(iter = m_setGroupOnLine.begin(); iter != m_setGroupOnLine.end(); ++iter)
   {
       fd = pUsr->m_mapUsrLogin[*iter].fd;

       if(fd == usrInfo.fd)
       {
           continue;
       }

       m_usrSend.fd = fd;
       MsgDequeSendPush(m_usrSend);

          printf("\n\n[ SEND CHATGROUP ]    Server----> GROUP ,  fd:%d\n", fd);
   }

   return true;
}

bool CMsgManage::UpdateOfflinePushInfo(uint32_t userId, uint32_t groupId)
{
    char buf[64];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf),"%d_%d",userId, groupId);

    CPublicFun* pFun = CPublicFun::GetInstance();
    string strNew ;

   pthread_mutex_lock(&pFun->m_mtxOffline);
   pFun->m_mapMsgOffline[buf] = strNew;
   pthread_mutex_unlock(&pFun->m_mtxOffline);

    return true;
}


bool  CMsgManage::GetGroupMember(const MarsMsgChat::SendMsgReq& msg)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT userid FROM tb_grouprelate WHERE groupId = %d",  atoi(msg.userto().c_str()));

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
                  printf(" [ ERROR ]  Sql:%s\n ",m_szSql);
                  return false;
             }
         }
     }

    return true;
}

void CMsgManage::PrintData(char* pData, int nLen)
{
    for(int i = 0; i < nLen; ++i)
    {
        if( ('0' <= pData[i] && pData[i] <='9') || ('a' <= pData[i] && pData[i] <='z')
|| ('A' <= pData[i] && pData[i] <='Z'))
        {
            printf("%c", pData[i]);
        }
        else
        {
            printf(" %d ", pData[i]);
        }
    }
    cout << endl;
}


