#ifndef _IMSGBASE_H_
#define _IMSGBASE_H_
#include "../public/PublicFun.h"

//server与client 消息交互定义
enum MarsMsgType
{
    //client -> server
    ENU_MARS_MSG_LOGIN =1,    //上线
    ENU_MARS_MSG_CONTACT_REQ_A,   //添加好友：A请求给server
    ENU_MARS_MSG_CHAT_PRIVATE ,    //  单聊
    ENU_MARS_MSG_CONTACT_RSP_B,	//		添加好友: B回应给server
    ENU_MARS_MSG_CHAT_GROUP_A,   //   群聊:  A请求给server
    ENU_MARS_MSG_KEEPLIVE = 6,          // 心跳
    ENU_MARS_MSG_SYSTEM_PUSH,   //  系统推送:
    ENU_MARS_MSG_LOGOUT,   //  下线:
    ENU_MARS_MSG_OFFLINE_B = 9,

    // server -> client
    ENU_MARS_MSG_CHAT_PRIVET_B =  1100,          // 单聊
    ENU_MARS_MSG_CONTACT_REQ_B,		//  添加好友: server转发给B
    ENUI_MARS_MSG_CONTACT_RSP_A,     //  添加好友：server将请求结果转发给A
    ENU_MARS_MSG_CHAT_GROUP_ALL,   // 群聊：server转发给群内所有人
    ENU_MARS_MSG_SYSTEM_PUSH_B,      // 系统推送：
    ENU_MARS_MSG_LOGOUT_DEVICE_A = 1105,
};

enum enContactReqType
{
    ENU_CONTACT_REQUEST = 1,	// A->Server->B  :请求添加好友
    ENU_CONTACT_RSP = 2,		//		B->server->A
};


#define MARS_MSG_HEAR_LEN 20
#define MARS_USRDATA_MAX  1024*10

const uint32_t PROTOBUF_VERSION = 200;

//protobuf数据接收格式
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

#define MARS_MSG_MAX_RECVDATA  sizeof(MARSPACKAGE)

typedef struct _st_mars_usr
{
    int fd;
    uint32_t nMsgLen;
    MARSPACKAGE usrPackage;
}USERDATAINFO,*PUSERDATAINFO;

#pragma pack()

//离线消息结构体定义
#define MAX_TEXT_SIZE 2000
typedef struct stOfflineMsg
{
    char msgId[16];
    uint32_t fromUsrId;
    uint32_t toUsrId;
    uint32_t msgType;
    uint32_t sendType;
    uint32_t len;
    uint32_t width;
    char conversationId[40];   //需大于32,否则会粘后面的text
    char createTs[24];
    char text[MAX_TEXT_SIZE];
}STOFFLINEMSG,*PSTOFFLINEMSG;


//数据库离线消息字段数
#define MSG_OFFLINE_DB_FIELD 10

//数据库用户信息字段数
#define MSG_USRINFO_DB_FIELD 7

//数据库用户信息字段数
#define MSG_GROUP_DB_FIELD 3

//离线消息推送类型
enum enOfflineMsgType
{
     ENU_MSG_OFFLINE_GROUP_CHAT =1,
    ENU_MSG_OFFLINE_PRIVATE_CHAT ,
    ENU_MSG_OFFLINE_ADD_CONTACT_REQ,
    ENU_MSG_OFFLINE_ADD_CONTACT_RSP,
};

//离线消息获取方式
enum enOfflineDealMethod
{
    ENU_MSG_OFFLINE_METHOD_BASE = 1,     //添加好友，好友回执，私聊
   ENU_MSG_OFFLINE_METHOD_GROUP_CHAT,   // 群聊
};

//消息推送类型
enum enMsgPushType
{
    ENU_MSG_PUSH_SYSTEM = 1,
    ENU_MSG_PUSH_USER ,
    ENU_MSG_PUSH_OFFLINE,
};

enum enMsgSendResult
{
    ENU_MSG_SUCCESS = 1000,
    ENU_MSG_FAILURE,
    ENU_MSG_OTHERS,
};

















#endif
