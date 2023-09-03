#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a new client connection
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Error in accepting");
            continue;
        }

        printf("parent process\n");

        // Fork a new process to handle the client
        int child_pid = fork();

        if (child_pid == 0) {
            // Child process handles the client
            close(server_socket); // Close the server socket in the child process

            printf("creating child process\n");

            while (1) {
                // Receive data from the client
                int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
                }

                buffer[bytes_received] = '\0';

                printf("Waiting for the message from the client::%s->%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                printf("Received Client Message: %s", buffer);

                // Check for "exit" command from client
                if (strcmp(buffer, "exit\n") == 0) {
                    printf("Closing the child process for client::%s->%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
                }

                // Send data back to the client
                printf("Sending back the message to the client::%s->%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                send(client_socket, buffer, strlen(buffer), 0);
            }

            // Close the client socket and exit the child process
            close(client_socket);
            exit(0);
        } else if (child_pid > 0) {
            // Parent process keeps track of child processes
            close(client_socket); // Close the client socket in the parent process
        } else {
            perror("Error in fork");
            continue;
        }
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

