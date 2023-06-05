#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 99
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    int num_bytes;

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

    // Send messages to the server
    while (1) {
        printf("Enter a message (enter 'stop' to stop the server): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Send the message to the server
        if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        // Receive the echoed message from the server
        memset(buffer, 0, sizeof(buffer));
        if ((num_bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
            printf("Echoed message from server: %s", buffer);
        } else if (num_bytes == 0) {
            printf("Server stopped.\n");
            break;
        } else {
            perror("recv failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close the socket
    close(client_fd);

    return 0;
}
