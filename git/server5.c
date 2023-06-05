#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/types.h>
#define PORT 9995
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port 9999
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        while (1) {
            // Receive the message from the client
            memset(buffer, 0, sizeof(buffer));
            if (recv(new_socket, buffer, sizeof(buffer), 0) < 0) {
                perror("recv failed");
                exit(EXIT_FAILURE);
            }

            printf("Received message from client: %s", buffer);

            // Send the message back to the client
            if (send(new_socket, buffer, strlen(buffer), 0) < 0) {
                perror("send failed");
                exit(EXIT_FAILURE);
            }

            // Check if the received message is the stop keyword
            if (strcmp(buffer, "stop\n") == 0) {
                printf("Received stop keyword. Closing connection.\n");
                break;
            }
        }

        close(new_socket);
    }

    close(server_fd);

    return 0;
}
