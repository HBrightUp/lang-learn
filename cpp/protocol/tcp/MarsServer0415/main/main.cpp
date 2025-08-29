#include<iostream>
#include"../epoll/EpollServer.h"
#include "../protobuf/chat.pb.h"
#include "../public/PublicFun.h"
#include "../msg/MsgManage.h"
#include "../mysql/EncapMysql.h"

using namespace std;

int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

     CEpollServer* pServer = CEpollServer::GetInstance();

    if(pServer->InitServer())
    {
        pServer->Run();
    }
    

    return 0;
}
