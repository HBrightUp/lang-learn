#include<iostream>
#include"EpollServer.h"
//#include "./chat.pb/chat.pb.h"

using namespace std;

int main(int argc, char** argv)
{
    CEpollServer theApp;

	

	
	



    if(theApp.InitServer("192.168.2.53", 8003))
    {
        theApp.Run();	
    }
    
    
    return 0;
}
