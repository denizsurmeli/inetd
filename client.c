#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 5000 

int main(int argc, char *argv[]) {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);  // Port number
    addr.sin_addr.s_addr = INADDR_ANY;  // Any available IP address

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    // read the first argument as the client id
    int client_id = atoi(argv[1]);
    printf("This is client #%d\n", client_id);
    
    while(1){
        // ask for a message to send to the server
        char message[1024];
        printf("Enter request(negative to terminate): ");
        fgets(message, 1024, stdin);

        // send the message to the server
        send(sockfd, message, strlen(message), 0);
        // if the message sent was a negative integer, terminate the connection
        if (atoi(message) < 0) {
            printf("Will terminate\n");

            // close the socket
            close(sockfd);
            break;
        }
        // receive the message back from the server
        char buffer[1024] = {0};
        read(sockfd, buffer, 1024);

        // print the message
        printf("\tResult: %s\n", buffer);


    }

    return 0;
}