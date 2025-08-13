#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include<iostream>
 
const int BUFFER_SIZE = 1024;
 
int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cout << "Please input port." << std::endl;
        return -1;
    }

    int port = atoi(argv[1]);
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
 
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  
    servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");  

    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cerr << "connect failed!" << std::endl;
        return -1;
    }
 
    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
 
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != nullptr)
    {
        send(sock_cli, sendbuf, strlen(sendbuf),0); 
        if(strcmp(sendbuf,"exit\n")==0)
            break;

        std::cout << "waiting data +++" << std::endl;
        size_t len = recv(sock_cli, recvbuf, sizeof(recvbuf),0); 
         std::cout << "waiting data ---" << std::endl;

        if (len <= 0) {
            break;
        }

        fputs(recvbuf, stdout);
 
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock_cli);

    return 0;
}