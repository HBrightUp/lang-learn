#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    sockaddr_in servaddr;
    char buffer[1024];

    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket creation failed" << std::endl;
        return 1;
    }

    // set address of server
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        std::cerr << "invalid address/ address not supported" << std::endl;
        return 1;
    }

    // send data
    const char *message = "Hello from client!";
    sendto(sockfd, message, strlen(message), 0, (const sockaddr *)&servaddr, sizeof(servaddr));
    std::cout << "Message sent." << std::endl;

    // receive response
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (sockaddr *)&servaddr, &len);
    if (n < 0) {
        std::cerr << "recvfrom failed" << std::endl;
        return 1;
    }
    
    buffer[n] = '\0';
    std::cout << "Received from server: " << buffer << std::endl;

    // close socket
    close(sockfd);

    return 0;
}