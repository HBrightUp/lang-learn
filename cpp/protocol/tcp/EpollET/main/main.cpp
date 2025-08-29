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

    CMsgManage::GetInstance();
    CPublicFun::GetInstance()->Init();

     CEpollServer theApp;
    if(theApp.InitServer())
    {
        theApp.Run();	
    }
    

    return 0;
}
