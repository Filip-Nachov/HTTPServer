#include <asm-generic/socket.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Could not create socket: %s...\n", strerror(errno));
        return 1;
    }

    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(&reuse)) < 0){
        printf("Could not reuse socket: %s...\n", strerror(errno));
    }

    return EXIT_SUCCESS;
}
