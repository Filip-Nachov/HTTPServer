#include <asm-generic/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 6969
#define MAX_SIZE 4096

void *handleClient(void *p_clientFd);
const char *getFileExtension(const char *file_name);
void buildResponse(const char *fileName, const char *fileExt, char *response, ssize_t *responseLen);
const char *getMIMEType(const char *file_ext);

const char *getMIMEType(const char *file_ext) {
    if (strcasecmp(file_ext, "html") == 0) {
        return "text/html";
    } else if (strcasecmp(file_ext, "txt") == 0) {
        return "text/plain";
    } else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0) {
        return "image/jpeg";
    } else if (strcasecmp(file_ext, "png") == 0) {
        return "image/png";
    } else {
        return "application/octet-stream";
    }
}

void buildResponse(const char *fileName, const char *fileExt, char *response, ssize_t *responseLen) {
    const char *mimeType = getMIMEType(fileExt);
}

const char *getFileExtension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (!dot || dot == file_name) {
        return "";
    }
    return dot + 1;
}

void *handleClient(void *p_clientFd) {
    int clientFd = *((int*)p_clientFd);
    free(p_clientFd);
    char buf[1024] = {0}, bufCopy[1024] = {0};
    ssize_t bytesReceived = recv(clientFd, buf, sizeof(buf), 0);

    if (bytesReceived < 0) {
        printf("Could not get buffer: %s...\n", strerror(errno));
        return NULL;
    }

    strcpy(bufCopy, buf);
    char *method = strtok(bufCopy, " ");
    char *filePath = strtok(NULL, " ");
    strtok(NULL, "\r\n");

    if (strcmp(method, "GET") != 0) {
        printf("Only accept GET...");
        return NULL;
    }

    char fileExt[32] = {0};
    char fileName[256];
    strcpy(fileExt, getFileExtension(filePath));
    strncpy(fileName, filePath, strlen(filePath) - strlen(fileExt) - 1);
    fileName[strlen(filePath) - strlen(fileExt) - 1] = '\0';

    char *response = (char *)malloc(MAX_SIZE * 2);
    ssize_t responseLen;
    buildResponse(fileName, fileExt, response, &responseLen);

    send(clientFd, response, responseLen, 0);

    printf("%s\n", fileExt);
    close(clientFd);
    return NULL;
}

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
    
    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientFd = accept(sockFd, (struct sockaddr*) &clientAddr, &clientAddrLen);

        if (clientFd < 0) {
            printf("Could not accept connection: %s...\n", strerror(errno));
            return 1;
        } 
        printf("Client Connected\n");
        pthread_t t;
        int *pClient = malloc(sizeof(int));
        *pClient = clientFd;
        pthread_create(&t, NULL, handleClient, pClient);
    }

    close(sockFd);

    return EXIT_SUCCESS;
}
