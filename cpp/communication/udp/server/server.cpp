#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int sockfd;
    sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;

    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
    std::cerr << "socket creation failed" << std::endl;
    return 1;
    }

    // set address of server
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    // bind socket address
    if (bind(sockfd, (const sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "bind failed" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    len = sizeof(cliaddr);  // init len

    while(true) {
        // receive data
        int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (sockaddr *)&cliaddr, &len);
        if (n < 0) {

            std::cerr << "recvfrom failed" << std::endl;
            return 1;
        }
        buffer[n] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        // send response
        const char *response = "Hello from server!";
        sendto(sockfd, response, strlen(response), 0, (const sockaddr *)&cliaddr, len);
        std::cout << "Response sent." << std::endl;
    }

    // close socket
    close(sockfd);

    return 0;
}