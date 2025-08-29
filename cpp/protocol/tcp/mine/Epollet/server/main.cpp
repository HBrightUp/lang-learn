#include<iostream>
#include"EpollServer.h"
//#include "./chat.pb/chat.pb.h"

using namespace std;

int main(int argc, char** argv)
{
    CEpollServer theApp;

	

	
	



    if(theApp.InitServer("192.168.2.45", 7004))
    {
        theApp.Run();	
    }
    
    
    return 0;
}
