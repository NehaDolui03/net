#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define SERVER_IP "127.0.0.1"
#define PORT 9999
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PORT);

    // Receive the date and time from the server
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }

    printf("Server Date and Time: %s", buffer);

    close(client_fd);

    return 0;
}
