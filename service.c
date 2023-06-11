#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SQUARE_PORT 5010
#define CUBE_PORT 5020


int square(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}


int main(int argc, char *argv[]) {
    int request = atoi(argv[1]);
    if (request < 0) {
        perror("Invalid request");
        exit(EXIT_FAILURE);
    }

    char* method =argv[2];
    if (strcmp(method, "square") != 0 && strcmp(method, "cube") != 0) {
        perror("Invalid method");
        exit(EXIT_FAILURE);
    }

    char* ip = argv[3];
    printf("Request: %d to the ip %s\n", request, ip);
    int result = strcmp(method, "square") == 0 ? square(request) : cube(request);
    int port = strcmp(method, "square") == 0 ? SQUARE_PORT : CUBE_PORT;

    // create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // connect to the client
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);  // Port number
    addr.sin_addr.s_addr = inet_addr(ip);  // Any available IP address

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to connect to client");
        exit(EXIT_FAILURE);
    }

    // send the result to the client
    char buffer[1024] = {0};
    sprintf(buffer, "%d", result);
    send(sockfd, buffer, strlen(buffer), 0);

    // close the socket
    close(sockfd);


    printf("%d\n", result);
    exit(EXIT_SUCCESS);
}