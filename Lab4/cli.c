#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    while (1) {
        // Read user input
        printf("Sending message to the server....\n");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send the message to the server
        send(client_socket, buffer, strlen(buffer), 0);

        // Check for "exit" command to terminate the client
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        // Receive and display the server's response
        printf("Waiting for message from the server....\n");
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            perror("Error in receiving data");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received Server Message: %s\n", buffer);
    }

    // Close the client socket and exit
    close(client_socket);
    printf("Disconnected from the server.\n");

    return 0;
}

