#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/types.h>

#define PORT 99
#define BUFFER_SIZE 1024

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    int num_bytes;

    while ((num_bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        // Echo the message back to the client
        send(client_fd, buffer, num_bytes, 0);
        memset(buffer, 0, sizeof(buffer));
    }

    if (num_bytes == 0) {
        printf("Client disconnected.\n");
    } else {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }

    close(client_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_address, client_address;
    int addrlen = sizeof(client_address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected. Client address: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Handle client in a separate process
        if (fork() == 0) {
            close(server_fd);
            handle_client(client_fd);
            exit(EXIT_SUCCESS);
        }

        close(client_fd);
    }

    return 0;
}
