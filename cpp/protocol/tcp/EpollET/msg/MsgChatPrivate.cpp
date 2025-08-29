#include "MsgChatPrivate.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"MsgManage.h"
#include"../usr/UserManage.h"
#include"../protobuf/chat.pb.h"





CMsgChatPrivate::CMsgChatPrivate()
{
    m_usBodyLen = 0;
    //m_uMsgType = ENU_MARS_MSG_CHAT_PRIVATE;
    memset(m_szSql, 0, sizeof(m_szSql));


}

CMsgChatPrivate::~CMsgChatPrivate()
{
 CPublicFun::GetInstance()->m_sqlCom.ReleaseOneConn(m_mySqlEncap);
}

void CMsgChatPrivate::Deal(const USERDATAINFO& Data)
{
    if(!m_mySqlEncap)
    {
        m_mySqlEncap =  static_cast<EncapMysql::CEncapMysql*>(CPublicFun::GetInstance()->m_sqlCom.GetOneConn());
    }

    MarsMsgChat::SendMsgReq msgReq;
    msgReq.ParseFromArray(Data.usrPackage.usrData, ntohl(Data.usrPackage.nBodyLen));

    printf("\n[Recv]   [Msg:   CHAT  client A----> server\n ]  ,  Socket:%d\n", Data.iSocket);
    printf("\n\n\naccess_token:%s, userFrom:%s,userTo:%s,text:%s,msgType:%d,headimage:%s,sendName:%s,senduserID:%s,conversation:%s,sendtype:%d\n\n\n",
           msgReq.access_token().c_str(),  msgReq.userfrom().c_str(), msgReq.userto().c_str(),msgReq.text().c_str(),msgReq.msgtype(),msgReq.headimage().c_str(),
           msgReq.sendname().c_str(),msgReq.senduserid().c_str(),msgReq.converstionid().c_str(), msgReq.sendtype());

    //返回给客户端的错误码
    int32_t  ierrorCode = 0;
    m_uUniqueMsgID =  CPublicFun::GetInstance()->GetSystemTime();

    //会话信息存入数据库
    if(StoreConversationID(msgReq))
    {
        //是否在线
       // int iSocket = GetRecvUsrSocket(msgReq.userto());
        int iSocket = CUsrManage::GetInstance()->GetUsrSocket(atoi(msgReq.userto().c_str()));
        if(iSocket > 0)
        {
            //转发消息给B,B收到信息后会有消息回执，这里不做失败处理
            TransmitMsgToB(msgReq, Data, iSocket);
        }
        else
        {
            ierrorCode = 10;				//B不在线
            //将消息记录存入到数据库
            if(!StoreMsgToDB(msgReq))
            {
                ierrorCode = 12;		//存储离线消息记录失败
            }
        }
    }
    else
    {
        ierrorCode = 11;		//存储会话信息失败
    }

    AckToA(msgReq, Data, ierrorCode);

    return ;
}

bool CMsgChatPrivate::AckToA(const MarsMsgChat::SendMsgReq& msgReq, const USERDATAINFO& Data,  const int32_t errorCode)
{
    printf("CSendMsgReq::AckToA() +++\n\n");


    STSENDMSGREQ  strReq;
    strReq.access_token = msgReq.access_token();
    strReq.userFrom = msgReq.userfrom();
    strReq.userTo = msgReq.userto();
    strReq.text = msgReq.text();
    strReq.msg_Type = msgReq.msgtype();
    strReq.headImage = msgReq.headimage();
    strReq.sendName = msgReq.sendname();
    strReq.sendUserID = msgReq.senduserid();
    strReq.converstionID = msgReq.converstionid();
    strReq.send_Type = msgReq.sendtype();


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
        msgRsp.set_msgid(m_uUniqueMsgID);


        string strTemp = "reserve";
        msgRsp.set_access_token(msgReq.access_token());


        if(!GetConversationID(msgReq,  strTemp))
        {
            strTemp = "unKnown";
        }
        msgRsp.set_converstionid(strTemp);

        msgRsp.set_msguniqueid(msgReq.msguniqueid());


        printf("\n[Send    ACK]   [Msg:   CHAT  Server----> Client A \n ]  ,  Socket:%d\n", Data.iSocket);
        printf("err_code:%d, errmsg:%d,msgid:%d,access_token:%s,converstionid:%d,msguniqueid:%d \n",
               msgRsp.err_code(),  msgRsp.errmsg(), msgRsp.msgid(),msgRsp.access_token(),msgRsp.converstionid(),msgRsp.msguniqueid());


         msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
         m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

        m_usrSendData.iSocket = Data.iSocket;

        m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
        m_usrSendData.usrPackage.nSeq = Data.usrPackage.nSeq;

        uTemp = ENU_MARS_MSG_CHAT_PRIVATE;
        m_usrSendData.usrPackage.nCmd = htonl(uTemp);

        uTemp = MARS_MSG_HEAR_LEN;
        m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

        uTemp = 200;
        m_usrSendData.usrPackage.nVersion = htonl(uTemp);

        CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

         CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

    return true;
}

 bool CMsgChatPrivate::TransmitMsgToB(const MarsMsgChat::SendMsgReq& msg, const USERDATAINFO& Data, const uint32_t iSocket )
 {
     MarsMsgChat::PushRsp rsp;

     int32_t uTemp = 2;
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
    
    rsp.set_msgid(m_uUniqueMsgID);

    printf("\n[Send ]   [Msg:   CHAT  Server----> Client B \n ]  ,  Socket:%d\n", iSocket);
    printf("msgpushtype:%d, errmsg:%d,state:%d,access_token:%s,userfrom:%s,userto:%s,text:%s,headimage:%s,sendname:%s,senduserid:%s,converstionid:%s,sendtype:%d,msgtype:%d,msgid:%d \n",
           rsp.msgpushtype(),  rsp.errmsg(), rsp.state(),rsp.accesstoken(),rsp.userfrom(),rsp.userto(),rsp.text(),rsp.headimage(),rsp.sendname(),rsp.senduserid(),
           rsp.converstionid(),rsp.sendtype(),rsp.msgtype(),rsp.msgid());

     rsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
     m_usrSendData.nMsgLen = rsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSendData.iSocket = iSocket;

    m_usrSendData.usrPackage.nBodyLen = htonl(rsp.ByteSize());

    uTemp = 0;
    m_usrSendData.usrPackage.nSeq = htonl(uTemp);

    uTemp = ENU_MARS_MSG_PUSH;
    m_usrSendData.usrPackage.nCmd = htonl(uTemp);

    uTemp = MARS_MSG_HEAR_LEN;
    m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSendData.usrPackage.nVersion = htonl(uTemp);


    CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

     CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

     return true;
 }


bool CMsgChatPrivate::StoreConversationID(const MarsMsgChat::SendMsgReq& msg)
{
    memset(m_szSql, 0, sizeof(m_szSql));
    //snprintf(m_szSql, sizeof(m_szSql),"INSERT INTO marsdatabase.tb_conversation(sendid, receiveid, conversationid, lastMsg, sendType) VALUES(%s, %s,'%s','%s', '%d')",\
             msg.userfrom().data(), msg.userto().data(), msg.converstionid().data(), msg.text().data(), msg.sendtype());

             //printf("\n%d,%d,'%s','%s',%d,%d\n", atoi(msg.userfrom().data()), atoi(msg.userto().data()), msg.converstionid().data(),  msg.text().data(),  msg.msgtype(), msg.sendtype());


             snprintf(m_szSql, sizeof(m_szSql),"call sp_conversation_save(%d,%d,'%s','%s',%d,%d, @ret )",\
                      atoi(msg.userfrom().data()), atoi(msg.userto().data()), msg.converstionid().data(),  msg.text().data(),  msg.msgtype(), msg.sendtype());

             printf("%s", m_szSql);

    return ExecuteMysql(m_szSql);
}
bool CMsgChatPrivate::StoreMsgToDB(const MarsMsgChat::SendMsgReq& msg)
{
    memset(m_szSql, 0, sizeof(m_szSql));
     int64_t uMsgID = CPublicFun::GetInstance()->GetSystemTime();


    snprintf(m_szSql, sizeof(m_szSql),"INSERT INTO tb_msg(msgid,fromUsrid,toUsrid,text,msgType,sendType,conversationid) VALUES(%d, %d, %d,%s,%d,%d,%s)",  \
             uMsgID, atoi(msg.userfrom().c_str()),atoi( msg.userto().c_str()), msg.text().c_str(), msg.msgtype(), msg.sendtype(), msg.converstionid().c_str());

    printf("CSendMsgReq::StoreMsgToDB(), szSql:%s\n\n",m_szSql);
    return ExecuteMysql(m_szSql);
}

bool CMsgChatPrivate::GetConversationID(const MarsMsgChat::SendMsgReq& msg, string& strCoversationID )
{
    memset(m_szSql, 0, sizeof(m_szSql));
    snprintf(m_szSql, sizeof(m_szSql),"SELECT conversationID FROM tb_contacts AS c WHERE c.uid = %s AND c.cuid = %s",  msg.userfrom().data(), msg.userto().c_str());

    // printf("CSendMsgReq::GetConversationID(), szSql:%s\n\n",m_szSql);


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
                  printf("GetConversationID failed!\n ");
                  return false;
             }
         }
     }

     return true;
}

#if 0
bool CMsgChatPrivate::ExecuteMysql(const char* szSql)
{
    int nRet = 0;
   if(m_mySqlEncap)
   {
       nRet = m_mySqlEncap->ModifyQuery(szSql);
       if(nRet != 0)
       {
           printf("SelectMysql() failed! sql:%s\n ", szSql);
           return false;
       }
   }
    return true;
}

#endif
