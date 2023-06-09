#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


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
    int result = strcmp(method, "square") == 0 ? square(request) : cube(request);

    printf("%d\n", result);
    exit(EXIT_SUCCESS);
}