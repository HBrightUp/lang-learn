#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<iostream>


const int BUFFER_LENGTH = 1024;

void* client_routine(void* arg){
    int clientfd=*(int*)arg;
    
    while(1){
        char buffer[BUFFER_LENGTH]= { 0 };
        memset(buffer, BUFFER_LENGTH, 0);

        std::cout << "waiting data +++" << std::endl;
        int len = recv(clientfd,buffer,BUFFER_LENGTH,0);
        std::cout << "waiting data ---" << std::endl;
        if(len > 0){
            std::cout << "Received data, len: " << len << ", data: " << buffer << std::endl;
            len = send(clientfd, buffer, strlen(buffer), 0);    
            std::cout << "send data, len: " << len << ", data: " << buffer << std::endl; 
        }else if(len == 0){
            std::cout << "client exit." << std::endl;
            close(clientfd);
            break;
        }else{
            
            close(clientfd);
            break;
        }
    }

    return nullptr;
}

int main(int argc,char** argv){

    if(argc < 2){
        std::cout << "Please input port." << std::endl;
        return -1;
    }

    int port=atoi(argv[1]);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    memset(&addr,0,sizeof(sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr= INADDR_ANY;   //  0.0.0.0

    if(bind(sockfd,(sockaddr*)&addr,sizeof(sockaddr_in))<0){
        perror("bind");
        return 2;
    }

    if(listen(sockfd,5)<0){
        perror("listen");
        return 3;
    }

    while(true){
        sockaddr_in client_addr;
        memset(&client_addr,0,sizeof(sockaddr_in));

        socklen_t client_len = sizeof(client_addr);
        int clientfd = accept(sockfd,(sockaddr*)&client_addr,&client_len);

        std::cout << "accpet client" << std::endl;

        pthread_t thread_id;
        pthread_create(&thread_id,NULL,client_routine,&clientfd); // one clien one thread
    }

    return 0;
}