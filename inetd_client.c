#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int port = atoi(argv[1]);
    if (port == 0) {
        perror("Invalid port number");
        exit(EXIT_FAILURE);
    }

    // create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // connect to the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);  // Port number
    addr.sin_addr.s_addr = INADDR_ANY;  // Any available IP address

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    // ask for the request to send to the server
    char request[1024];
    printf("Enter request: ");
    fgets(request, 1024, stdin);

    // send the request to the server
    send(sockfd, request, strlen(request), 0);

    // receive the message back from the server
    char buffer[1024] = {0};
    read(sockfd, buffer, 1024);

    // print the message
    printf("\tResult: %s\n", buffer);

    // close the socket
    close(sockfd);

    return 0;
}