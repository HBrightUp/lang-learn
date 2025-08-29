#ifndef _IMSGBASE_H_
#define _IMSGBASE_H_
#include "../public/PublicFun.h"
#include "IMsgBase.h"
#include "../mysql/MysqlCom.h"
#include "../mysql/EncapMysql.h"



class IMsgBase;


enum MarsMsgType
{
    ENU_MARS_MSG_LOGIN =1,	//
    ENU_MARS_MSG_CONTACT_REQ_A,	//	client A->server, ack:server-> A
    ENU_MARS_MSG_CHAT_PRIVATE ,    //  client to server
    ENU_MARS_MSG_CONTACT_RSP,	//		B->server for add contact rsp
    ENU_MARS_MSG_CHAT_GROUP,
    ENU_MARS_MSG_KEEPLIVE = 6,          // client to server  for keeplive
    ENU_MARS_MSG_SYSTEM_PUSH,


    // 1~1000:client  -> server


    // >1000:server -> client
    ENU_MARS_MSG_PUSH =  1100,          // server to cleint for push msg


    //无单独类，在ENU_MARS_MSG_CONTACT_ADD中处理
    ENU_MARS_MSG_CONTACT_REQ_B,		//  for add contact
    ENUI_MARS_MSG_CONTACT_RSP_A,
    ENU_MARS_MSG_CHAT_GROUP_ALL,
    ENU_MARS_MSG_SYSTEM_PUSH_B,

};



enum MarsMsgTypePos
{
    ENU_MARS_MSG_KEEPLIVE_POS =  1,          // client to server  for keeplive
     ENU_MARS_MSG_CHAT_PRIVATE_POS  ,    //  client to server
    ENU_MARS_MSG_PUSH_POS,          // server to cleint for push msg
    ENU_MARS_MSG_LOGIN_POS,
    ENU_MARS_MSG_CONTACT_REQ_A_POS,	//client to server:
    ENU_MARS_MSG_CONTACT_RSP_POS,
    ENU_MARS_MSG_CHAT_GROUP_POS,       //
};

enum enContactReqType
{
    ENU_CONTACT_REQUEST = 1,	// A->Server->B  :请求添加好友
    ENU_CONTACT_RSP = 2,		//		B->server->A
};


#define MARS_MSG_HEAR_LEN 20
#define MARS_USRDATA_MAX  1024*10
#pragma pack(1)

typedef struct _st_mars_package
{
    uint32_t nHeadLen;
    uint32_t nVersion;
    uint32_t nCmd;
    uint32_t nSeq;
    uint32_t nBodyLen;
    char usrData[MARS_USRDATA_MAX];
}MARSPACKAGE,*PMARSPACKAGE;

typedef struct _st_mars_usr
{
    int iSocket;
    uint32_t nMsgLen;
    MARSPACKAGE usrPackage;
}USERDATAINFO,*PUSERDATAINFO;


struct ST_MSGPTR
{
    unsigned int pos;
    IMsgBase* Msgptr;
};

#pragma pack(1)



//登陆消息LoginRequst
typedef struct _st_MsgLogin_
{
    string  userName	;
    uint32_t userID;
    string email;
    string telPhone;
    string password;
    string imagePath ;
    string  CustionJson;
}STMARSLOGIN,*PSTMARSLOGIN;

//单聊SendMsgReq
typedef struct _st_SendMsgReq_
{
    string  access_token;
    string userFrom;
    string userTo;
    string text;
    int32_t msg_Type ;
    string  headImage;
    string  sendName;
    string  sendUserID;
    string converstionID;
   int32_t  send_Type;
}STSENDMSGREQ,*PSTSENDMSGREQ;











class IMsgBase
{

public:
    IMsgBase() {}
    virtual ~IMsgBase() {}

public:
    virtual void Deal(const USERDATAINFO& Data){}
    void SetCmdId(uint cmd) { m_uCmdId = cmd;}

protected:
    bool ExecuteMysql(const char* szSql);



public:
    USERDATAINFO m_usrSendData;
    //unsigned int m_uMsgType;
    unsigned short m_usBodyLen;
    unsigned int  m_uMsgCmd;
    char  m_szSql[1024];
    EncapMysql::CEncapMysql* m_mySqlEncap;

protected:
    uint  m_uCmdId;


};






















#endif
