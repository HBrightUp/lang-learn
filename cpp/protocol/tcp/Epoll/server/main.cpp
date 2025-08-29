#include<iostream>
#include"EpollServer.h"

using namespace std;

int main(int argc, char** argv)
{
    CEpollServer theApp;
    if(theApp.InitServer("192.168.1.103", 8000))
    {
        theApp.Run();	
    }
    
    
    return 0;
}
