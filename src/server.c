#include <asm-generic/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 6969

int main(int argc, char *argv[]) {
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1) {
        printf("Could not create socket: %s...\n", strerror(errno));
        return 1;
    }

    int reuse = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(&reuse)) < 0){
        printf("Could not reuse socket: %s...\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in servAddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT),
    };

    if (bind(sockFd, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
        printf("Could not bind socket: %s...\n", strerror(errno));
        return 1;
    }

    int backlog = 5;
    if (listen(sockFd, backlog) < 0) {
        printf("Listen failed: %s...\n", strerror(errno)); 
        return 1;
    }

    printf("Waiting for a client to connect...\n");
    
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientFd = accept(sockFd, (struct sockaddr*) &clientAddr, &clientAddrLen);
    
    if (clientFd < 0) {
        printf("Could not accept connection: %s...\n", strerror(errno));
        return 1;
    }

    printf("Client Connected\n");

    close(clientFd);
    close(sockFd);

    return EXIT_SUCCESS;
}
