#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 9999
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in server_address, client_address;
    int addrlen = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
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

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Receive the message from the client
        memset(buffer, 0, sizeof(buffer));
        if (recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, (socklen_t *)&addrlen) < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        printf("Received request from client: %s", buffer);

        // Get current date and time
        time_t current_time;
        time(&current_time);
        char *time_string = ctime(&current_time);

        // Send the date and time to the client
        if (sendto(server_fd, time_string, strlen(time_string), 0, (struct sockaddr *)&client_address, addrlen) < 0) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
