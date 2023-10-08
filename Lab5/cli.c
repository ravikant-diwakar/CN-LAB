#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Receive welcome message from server
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received message from server:\n%s\n", buffer); // Start on a new line

    while (1) {
        printf("Send message to server: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Remove newline character

        // Send message to server
        send(client_socket, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "bye") == 0) {
            printf("Connection Terminated\n");
            break;
        }

        // Receive reply from server
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("\nReceived message from server:%s\n", buffer); // Start on a new line
    }

    close(client_socket);
    return 0;
}
