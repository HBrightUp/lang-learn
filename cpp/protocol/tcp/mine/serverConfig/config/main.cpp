#include <iostream>\
#include<cstring>
#include"ServerConfig.h"

using namespace std;

int main()
{
     CServerConfig  server;
     server.SetFilePath("./config.conf");
     server.Parse();


    return 0;
}

