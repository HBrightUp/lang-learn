#include "IMsgBase.h"
#include "../protobuf/login.pb.h"
#include "../protobuf/chat.pb.h"
#include "MsgLogin.h"
#include "../public/PublicFun.h"
#include <arpa/inet.h>
#include"../msg/MsgManage.h"
#include "../usr/UserManage.h"
#include "MsgPush.h"



CMsgLogin::CMsgLogin()
{
    m_usBodyLen = 0;
 //   m_uMsgType = ENU_MARS_MSG_LOGIN;

}

CMsgLogin::~CMsgLogin()
{

}
void CMsgLogin ::Deal(const USERDATAINFO& Data)
{
    MarsLogin::LoginRequst  MsgLogin;

    MsgLogin.ParseFromArray(Data.usrPackage.usrData, ntohl(Data.usrPackage.nBodyLen));


    STMARSLOGIN  stLogin;
    stLogin.userName = MsgLogin.username();
    stLogin.userID = MsgLogin.userid();
    stLogin.email = MsgLogin.email();
    stLogin.telPhone = MsgLogin.telphone();
    stLogin.password = MsgLogin.password();
    stLogin.imagePath = MsgLogin.imagepath();
    stLogin.CustionJson = MsgLogin.custionjson();

    printf("\n[Recv]   [Msg:   Login   client ----> server ]  ,  Socket:%d\n", Data.iSocket);
     printf("username:%s, userid:%d, email:%s, telphone:%s, password:%s, imagepath:%s, custionjson:%s\n ",
            MsgLogin.username(), MsgLogin.userid(), MsgLogin.email(), MsgLogin.telphone(), MsgLogin.password(), MsgLogin.imagepath(), MsgLogin.custionjson());


    MarsMsgChat::SendMsgRsp msgRsp;
    msgRsp.set_err_code(1000);

    uint32_t iUserID = ntohl(MsgLogin.userid());
    uint32_t uTemp = 0;
    msgRsp.set_errmsg(uTemp);

    msgRsp.set_msgid(0);

    string strTemp = "reserve";
    msgRsp.set_access_token(strTemp);

    strTemp = "reserve";
    msgRsp.set_converstionid(strTemp);


    printf("\n[Send]   [Msg:   Login  Server----> Client ]  ,  Socket:%d\n", Data.iSocket);
     printf("err_code:%d, errmsg:%d, msgid:%d, access_token:%d,converstionid:%d\n ",msgRsp.err_code(), msgRsp.errmsg(), msgRsp.msgid(), msgRsp.access_token(), msgRsp.converstionid());


     msgRsp.SerializeToArray(m_usrSendData.usrPackage.usrData, sizeof(m_usrSendData.usrPackage.usrData));
     m_usrSendData.nMsgLen = msgRsp.ByteSize()  + MARS_MSG_HEAR_LEN;

    m_usrSendData.iSocket = Data.iSocket;

    m_usrSendData.usrPackage.nBodyLen = htonl(msgRsp.ByteSize());
    m_usrSendData.usrPackage.nSeq = Data.usrPackage.nSeq;

    uTemp = ENU_MARS_MSG_LOGIN;
    m_usrSendData.usrPackage.nCmd = htonl(uTemp);

     uTemp = MARS_MSG_HEAR_LEN;
    m_usrSendData.usrPackage.nHeadLen = htonl(uTemp);

    uTemp = 200;
    m_usrSendData.usrPackage.nVersion = htonl(uTemp);

    STUSRLOGIN stUsr;
    stUsr.iSocket = Data.iSocket;

    //login ack
    CMsgManage::GetInstance()->MsgDequeSendPush(m_usrSendData);

     CPublicFun::GetInstance()->AwakenWriteEvent(m_usrSendData.iSocket);

     //add usr
    CUsrManage::GetInstance()->AddUsr(MsgLogin.userid(), stUsr);

    //push msg if exist
   // CMsgPush* pMsgPush = static_cast<CMsgPush*>(CMsgManage::GetInstance()->GetMsgPtr(ENU_MARS_MSG_PUSH_POS));
    //pMsgPush->PushOffLineMsg(iUserID,  stUsr.iSocket);

    return ;
}
