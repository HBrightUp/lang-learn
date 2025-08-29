#include "EpollServer.h"

using namespace std;


CEpollServer::CEpollServer()
{
}

CEpollServer::~CEpollServer()
{
}

bool CEpollServer::InitServer(const char* strIP, int nPort)
{
    m_nEpollFd = epoll_create(MAX_SOCKFD_CONNECT);

    int opts =  O_NONBLOCK;
    if(fcntl(m_nEpollFd, F_SETFL, opts) < 0)
    {
        printf("Set NONBLOCK socket failed!\n");
        return false;
    }

    m_nSock = socket(AF_INET, SOCK_STREAM, 0);
    if(0 > m_nSock)
    {
        printf("socket error!\n");
        return false;
    }

    sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(nPort);
    //listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_addr.s_addr = inet_addr(strIP);

    int nReuseadd_on = 1;
    setsockopt(m_nSock, SOL_SOCKET, SO_REUSEADDR, &nReuseadd_on, sizeof(nReuseadd_on));

    if(bind(m_nSock, (sockaddr*)&listen_addr, sizeof(listen_addr)) != 0)
    {
        printf("bind error!\n");
        return false;
    }

    if(listen(m_nSock, 20) < 0)
    {
        printf("listen error!\n");
        return false;   
    }
    else
    {
        printf("Server listenning...\n");
    }

    if(pthread_create(&m_thListenThreadId, 0, (void* (*)(void*))ListenThread, this) != 0)
    {
        printf("create listen thread failed!\n");
        return false;
    }

    return true;
}

void CEpollServer::ListenThread(void* lpParam)
{
    CEpollServer* pEpollServer = (CEpollServer*)lpParam;

    sockaddr_in remote_addr;
    int len = sizeof(remote_addr);
    
    while(true)
    {
	printf("Start accept client connect!\n");
        int client_socket = accept(pEpollServer->m_nSock,(sockaddr*)&remote_addr, (socklen_t*)&len);
        if(client_socket < 0)
        {
            printf("Server Accept failed!client_socket:%d\n", client_socket);
            continue;
        }
        else
        {
            struct epoll_event ev;
            ev.events = EPOLLIN;
	    //ssev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
            ev.data.fd = client_socket;
            epoll_ctl(pEpollServer->m_nEpollFd, EPOLL_CTL_ADD, client_socket, &ev);
        }
    }
}


void CEpollServer::Run()
{
    while(true)
    {
        struct epoll_event events[MAX_SOCKFD_CONNECT];
        int nfds = epoll_wait(m_nEpollFd, events, MAX_SOCKFD_CONNECT, -1);

        for(int i = 0; i < nfds; ++i)
        {
            int client_socket = events[i].data.fd;
            char buffer[MAX_BUFFER_SIZE];
            memset(buffer, 0, sizeof(buffer));
            if(events[i].events & EPOLLIN)
            {   
                int rev_size = recv(events[i].data.fd, buffer, MAX_BUFFER_SIZE, 0);
                if(rev_size <= 0)
                {
                    printf("recv error!recv size:%d\n", rev_size);
                    struct epoll_event event_del;
                    event_del.data.fd = events[i].data.fd;
                    event_del.events = 0;
                    epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, event_del.data.fd, &event_del);
                }
                else
                {
                    printf("rev_size:%d,Recv:%s\n", rev_size,buffer);
                    struct epoll_event ev;
                    ev.events = EPOLLOUT;
                    ev.data.fd = client_socket;
                    epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, client_socket, &ev);
                }

             }
             else if(events[i].events & EPOLLOUT)
             {
                char sendbuff[MAX_BUFFER_SIZE];
                sprintf(sendbuff, "Good client! fd:%d\n", client_socket);
		
                int sendsize = send(client_socket, sendbuff, strlen(sendbuff) + 1, MSG_NOSIGNAL);
                if(sendsize <= 0)
                {
                    struct epoll_event event_del;
                    event_del.data.fd  = events[i].data.fd;
                    event_del.events = 0;
                    epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, event_del.data.fd, &event_del);
                }
                else
                {
                    printf("Send:%s\n", sendbuff);
                    struct epoll_event ev;
                    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                    ev.data.fd = client_socket;
                    epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, client_socket, &ev);
                }
              }
              else
              {
                  printf("EPOLL Error\n");
                  epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
              }
	}
    }
}


