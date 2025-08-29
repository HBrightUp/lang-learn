#include "EpollClient.h"

int main(int argc, char** argv)
{
    CEpollClient* pEpollClient = new CEpollClient(1000, "192.168.1.103", 8000);
    if(NULL != pEpollClient)
    {
        pEpollClient->Run();

        delete pEpollClient;
        pEpollClient = NULL;
    }

    

    return 0;
}
