#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define SQUARE_PORT 5010
#define CUBE_PORT 5020


void listen_port(int port) {
    /**
     * Listen for incoming connections on the given port.
     * If the connection is coming from port 5010, a square process will be spawned to handle the connection.
     * If the connection is coming from port 5020, a cube process will be spawned to handle the connection.
    */
    // for logging purposes
    char *proc_name = port == SQUARE_PORT ? "square" : "cube";

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a specific IP address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);  // Port number
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

    // Accept incoming connections
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd == -1) {
            perror("Failed to accept connection");
            continue;
        }

        printf("(inetd) Connection request to %s service\n", proc_name);

        // read the message from client and copy it in buffer
        char buffer[1024] = {0};
        read(client_sockfd, buffer, 1024);
        // trim the newline from the end of the message
        buffer[strcspn(buffer, "\n")] = 0;

        // print the message
        printf("(%s) Request: %s\n", proc_name, buffer);

        char *method = port == SQUARE_PORT ? "square" : "cube";
        // start the process named service with parameters: and the request and method
        char *service = "./service";
        char *request = buffer;
        char *ip = inet_ntoa(client_addr.sin_addr);
        // concat the arguments into a single string
        char *command[256];

        sprintf(command, "%s %s %s %s", service, request, method, ip);
        printf("(%s) Executing command: %s\n", proc_name, command);
        // release the client socket
        close(client_sockfd);

        // spawn a process to handle the request
        FILE *pipe = popen(command, "r");
        if (!pipe) {
            perror("Failed to spawn process");
            exit(EXIT_FAILURE);
        }

        // read the output of the process
        char result[1024];
        fgets(result, sizeof(result), pipe);

        // trim the newline from the end of the message
        result[strcspn(result, "\n")] = 0;

        // print the result
        printf("(%s) Reply sent as: %s. Terminating... \n", proc_name, result);

        // close the pipe
        pclose(pipe);
    }

}

int main() { 
    // spawn two threads to listen on ports 5010 and 5020
    pthread_t square_thread, cube_thread;
    pthread_create(&square_thread, NULL, listen_port, SQUARE_PORT);
    pthread_create(&cube_thread, NULL, listen_port, CUBE_PORT);

    printf("(inetd) Listening on ports 5010 and 5020...\n");
    // wait for the threads to finish
    pthread_join(square_thread, NULL);
    pthread_join(cube_thread, NULL);

    return 0;
}