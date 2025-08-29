#include "EpollClient.h"

int main(int argc, char** argv)
{
    CEpollClient* pEpollClient = new CEpollClient(1, "192.168.2.45", 7004);
    if(NULL != pEpollClient)
    {
        pEpollClient->Run();

        delete pEpollClient;
        pEpollClient = NULL;
    }

    

    return 0;
}
