#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == 0) {
        printf("Listening...\n");
    } else {
        perror("Listening failed");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &addr_size);
    if (new_socket < 0) {
        perror("Acceptance failed");
        exit(1);
    }
    printf("You got a connection from client: %s\n", inet_ntoa(new_addr.sin_addr));

    // Send welcome message to client
    char welcome_msg[] = "Welcome to my server";
    send(new_socket, welcome_msg, sizeof(welcome_msg), 0);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(new_socket, buffer, sizeof(buffer), 0);

        printf("Received Message from the Client: %s\n", buffer);

        if (strcmp(buffer, "bye") == 0) {
            printf("Connection Terminated\n");
            break;
        }

        // Send a reply to the client
        printf("Send message to client :");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Remove newline character
        send(new_socket, buffer, sizeof(buffer), 0);
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
