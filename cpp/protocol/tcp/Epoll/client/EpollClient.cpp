#include "EpollClient.h"

CEpollClient::CEpollClient(int iUserCount, const char* pIP, int iPort)
{
    strcpy(m_ip, pIP);
    m_iPort = iPort;
    m_iUserCount = iUserCount;
    m_iEpollFd = epoll_create(MAX_SOCKFD_COUNT);
    m_pAllUserStatus = (struct UserStatus*)malloc(iUserCount * sizeof(struct UserStatus));

    for(int iuserid = 0; iuserid < iUserCount; ++iuserid)
    {
        m_pAllUserStatus[iuserid].iUserStatus = FREE;
        sprintf(m_pAllUserStatus[iuserid].cSendbuff,"Client:%d,Hello Server!\n",iuserid);
        m_pAllUserStatus[iuserid].iBuffLen = strlen(m_pAllUserStatus[iuserid].cSendbuff) + 1;
        m_pAllUserStatus[iuserid].iSockFd = -1;
    }

    memset(m_iSockFd_UserId, 0xFF, sizeof(m_iSockFd_UserId));
}

CEpollClient::~CEpollClient()
{
    free(m_pAllUserStatus);
    m_pAllUserStatus = NULL;
}

int CEpollClient::ConnectToServer(int iUserId, const char *pServerIp, unsigned short uServerPort)
{
    if((m_pAllUserStatus[iUserId].iSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("CEpolllient socket init failed!\n");
        m_pAllUserStatus[iUserId].iSockFd = -1;
        return m_pAllUserStatus[iUserId].iSockFd;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(uServerPort);
    addr.sin_addr.s_addr = inet_addr(pServerIp);

    //int ireuseadd_on = 1;
    //setsockopt(m_pAllUserStatus[iUserId].iSockFd, SOL_SOCKET, SO_REUSEADDR, &ireuseadd_on,
    //sizeof(ireuseadd_on));

    unsigned long ul = 1;
    ioctl(m_pAllUserStatus[iUserId].iSockFd, FIONBIO, &ul);
    int nRet = connect(m_pAllUserStatus[iUserId].iSockFd, (struct sockaddr*)&addr, sizeof(addr));

    if( nRet== 0)
    {
        printf("connect success! iUserId:%d\n", iUserId);
    }
    else if(nRet == -1)
    {
        if(errno == EINPROGRESS)
        {
	    printf("connectint...! iUserId:%d\n", iUserId);
        }
        
    }
    else
    {
        printf("connectint failed! iUserId:%d\n", iUserId);
        return -1;
    }

    m_pAllUserStatus[iUserId].iUserStatus = CONNECT_OK;
    m_pAllUserStatus[iUserId].iSockFd = m_pAllUserStatus[iUserId].iSockFd;

    return m_pAllUserStatus[iUserId].iSockFd;
}

int CEpollClient::SendToServerData(int iUserId)
{
    sleep(1);
    int isendsize = -1;
    if(CONNECT_OK == m_pAllUserStatus[iUserId].iUserStatus || RECV_OK == m_pAllUserStatus[iUserId].iUserStatus)
    {
        isendsize = send(m_pAllUserStatus[iUserId].iSockFd, m_pAllUserStatus[iUserId].cSendbuff, m_pAllUserStatus[iUserId].iBuffLen, MSG_NOSIGNAL);

        if(isendsize < 0)
        {
            printf("SendToServerData failed! \n");
            return -1;
        }
        else
        {
            printf("[Send] Client:%d,isendsize:%d,Content:%s",iUserId, isendsize,m_pAllUserStatus[iUserId].cSendbuff);
            m_pAllUserStatus[iUserId].iUserStatus = SEND_OK;
        }
    }

    return isendsize;
}

int CEpollClient::RecvFromServer(int iUserId, char* pRecvBuff, int iBuffLen)
{
    int irecvsize = -1;
    if(SEND_OK == m_pAllUserStatus[iUserId].iUserStatus)
    {
        irecvsize = recv(m_pAllUserStatus[iUserId].iSockFd, pRecvBuff, iBuffLen, 0);
        if(0 > irecvsize)
        {
            printf("RecvFromServer failed!\n");
            return -1;
        }
        else if(0 == irecvsize)
        {
            printf("RecvFromServer for zero, the connection is off!iUserId:%d\n", iUserId);
            return -1;
        }
        else
        {
            printf("[Recv] Client:%d,Content:%s\n", iUserId,pRecvBuff);
            m_pAllUserStatus[iUserId].iUserStatus = RECV_OK;
        }
    }

    return irecvsize;
}

bool CEpollClient::CloseUser(int iUserId)
{
    close(m_pAllUserStatus[iUserId].iSockFd);
    m_pAllUserStatus[iUserId].iUserStatus = FREE;
    m_pAllUserStatus[iUserId].iSockFd = -1;

    return true;
}

int CEpollClient::Run()
{
    int isocketfd = -1;
    for(int iuserid = 0; iuserid < m_iUserCount; iuserid++)
    {
        struct epoll_event event;
        isocketfd = ConnectToServer(iuserid, m_ip, m_iPort);
        if(isocketfd < 0)
        {
            printf("ConnectToServer failed!\n");
            continue;
        }

        m_iSockFd_UserId[isocketfd] = iuserid;

        event.data.fd = isocketfd;
        event.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;

        m_pAllUserStatus[iuserid].uEpollEvents = event.events;
        epoll_ctl(m_iEpollFd, EPOLL_CTL_ADD, event.data.fd, &event);
    }

    while(1)
    {
        struct epoll_event events[MAX_SOCKFD_COUNT];
        char buffer[MAX_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int nfds = epoll_wait(m_iEpollFd, events, MAX_SOCKFD_COUNT, 100);

        for(int ifd = 0; ifd < nfds; ifd++)
        {
            struct epoll_event event_nfds;
            int iclientsockfd = events[ifd].data.fd;

            int iuserid = m_iSockFd_UserId[iclientsockfd];
            if(events[ifd].events & EPOLLOUT)
            {
                int iret = SendToServerData(iuserid);
                if(0 < iret)
                {
                    event_nfds.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                    event_nfds.data.fd = iclientsockfd;
                    epoll_ctl(m_iEpollFd, EPOLL_CTL_MOD, event_nfds.data.fd, &event_nfds);
                }
                else
                {
                    printf("SendToServerData failed!iret:%d,iuserid:%d", iret, iuserid);
                    DelEpoll(events[ifd].data.fd);
                    CloseUser(iuserid);
                }
            }
            else if(events[ifd].events & EPOLLIN)
            {
                int ilen = RecvFromServer(iuserid, buffer, MAX_BUFFER_SIZE);
                if(0 > ilen)
                {
                    printf("RecvFormServer failed!\n");
                    DelEpoll(events[ifd].data.fd);
                    CloseUser(iuserid);
                }
                else if(0 == ilen)
                {
                    printf("Receive Data is zero,disconnected!iuserid:%d\n", iuserid);
                    DelEpoll(events[ifd].data.fd);
                    CloseUser(iuserid);
                }
                else
                {
                    m_iSockFd_UserId[iclientsockfd] = iuserid;
                    event_nfds.data.fd = iclientsockfd;
                    event_nfds.events = EPOLLOUT | EPOLLERR |EPOLLHUP;
                    epoll_ctl(m_iEpollFd, EPOLL_CTL_MOD, event_nfds.data.fd, &event_nfds);
                }
            }
            else
            {
                printf("Otherepoll error!\n");
                DelEpoll(events[ifd].data.fd);
                CloseUser(iuserid);
            }
        }
    }
}

bool CEpollClient::DelEpoll(int iSockFd)
{
    bool bret = false;
    struct epoll_event event_del;
    if(0 < iSockFd)
    {
        event_del.data.fd = iSockFd;
        event_del.events = 0;
        if(0 == epoll_ctl(m_iEpollFd, EPOLL_CTL_DEL, event_del.data.fd, &event_del))
        {
            bret = true;
        }
        else
        {
            printf("DelEpoll failed!iSockFd:%d\n", iSockFd);
        }

        m_iSockFd_UserId[iSockFd] = -1;
    }

    return true;
}
