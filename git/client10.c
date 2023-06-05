#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4
#define BUFFER_SIZE 256

// Function to send frames to the server
void sendFrames(int serverSocket) {
    int sequenceNumber = 0;
    char buffer[BUFFER_SIZE];

    while (true) {
        // Read input message from the user
        printf("Enter message to send (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Check if user wants to quit
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // Create frame with sequence number
        buffer[0] = sequenceNumber;

        // Send frame to the server
        send(serverSocket, buffer, strlen(buffer) + 1, 0);

        // Receive acknowledgment from the server
        int ack;
        ssize_t bytesRead = recv(serverSocket, &ack, sizeof(ack), 0);

        if (bytesRead > 0) {
            printf("Received acknowledgment for frame %d\n", ack);
        } else {
            // Error or connection closed by the server
            break;
        }

        sequenceNumber++;
    }
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(80);

    // Connect to the server
    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Send frames using sliding window protocol
    sendFrames(serverSocket);

    // Close the socket
    close(serverSocket);

    return 0;
}
