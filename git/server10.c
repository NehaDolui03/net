#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4
#define BUFFER_SIZE 256

// Function to process frames received from the client
void processFrames(int clientSocket) {
    int expectedSequenceNumber = 0;
    int windowStart = 0;
    char buffer[BUFFER_SIZE];

    while (true) {
        // Receive frame from the client
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            // Extract sequence number from the frame
            int sequenceNumber = buffer[0];

            // Check if the current frame's sequence number is as expected
            if (sequenceNumber == expectedSequenceNumber) {
                // Process the frame
                printf("Processing frame %d: %s\n", sequenceNumber, buffer + 1);

                // Send acknowledgment to the client
                send(clientSocket, &sequenceNumber, sizeof(sequenceNumber), 0);

                expectedSequenceNumber++;
                windowStart++;
            } else {
                // Frame sequence number mismatch, resend last acknowledgment
                int lastAck = expectedSequenceNumber - 1;
                send(clientSocket, &lastAck, sizeof(lastAck), 0);
            }
        } else {
            // Error or connection closed by the client
            break;
        }
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(80);

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == -1) {
        perror("Socket listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    socklen_t clientAddrLen = sizeof(clientAddr);

    // Accept incoming connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        perror("Socket accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected: %s\n", inet_ntoa(clientAddr.sin_addr));

    // Process frames using sliding window protocol
    processFrames(clientSocket);

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
