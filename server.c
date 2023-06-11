#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 5000


int square(int x) {
    return x * x;
}

int main() {
    // Create a socket
    int child_proc_count = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a specific IP address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);  // Port number
    addr.sin_addr.s_addr = INADDR_ANY;  // Any available IP address

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("Failed to listen for connections");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port 5000...\n");

    // Accept incoming connections
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd == -1) {
            perror("Failed to accept connection");
            continue;
        }

        // Handle the connection
        printf("Accepted a connection.\n");

        // read the message from client and copy it in buffer
        int pid = fork();

        ++child_proc_count;
        if (pid == 0){
            // listen message from the client all the time
            while(1){
                char buffer[1024] = {0};
                read(client_sockfd, buffer, 1024);
                // print the message
 

                // check if the message is a negative integer
                if (atoi(buffer) < 0) {
                    printf("(child #%d) Request: %s, will terminate", child_proc_count, buffer);                    
                    // close the socket
                    close(client_sockfd);
                    exit(0);
                } else {
                    // send the message to the client
                    printf("(child #%d) Request: %s", child_proc_count, buffer);
                    char message[1024];
                    sprintf(message, "%d", square(atoi(buffer)));
                    send(client_sockfd, message, strlen(message), 0);
                }
            }

        } else {
            // print the pid of the child process
            printf("(child #%d) Child process created with pid: %d\n", child_proc_count, pid);
        }
    }

    // Close the server socket
    close(sockfd);

    return 0;
}
