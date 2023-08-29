#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Check the number of command-line arguments
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[250];

    // Create a socket using IPv4 and TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections, with a backlog of 10
    if (listen(server_fd, 10) == -1) {
        perror("Listening failed");
        close(server_fd);
        return 1;
    }

    printf("Listening on port %s.\nWaiting for connection from client...\n", argv[1]);

    // Accept an incoming connection
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Accepting failed");
        close(server_fd);
        return 1;
    }

    // Display connection details
    printf("Connection from %s:%d.\nAccepted new socket details %s:%s.\nReceived request message from client: ",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),
           inet_ntoa(server_addr.sin_addr), argv[1]);
    
    // Receive data from the client
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        perror("Error receiving message");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    buffer[bytes_received] = '\0';
    printf("%s \nEnter the response message to the client: ", buffer);

    // Read response from the user
    fgets(buffer, sizeof(buffer), stdin);
    // Send response to the client
    send(client_fd, buffer, strlen(buffer), 0);
    printf("Done sending message: %s \nWaiting for connection from client...\n", buffer);

    // Close the client and server sockets
    close(client_fd);
    close(server_fd);
    return 0;
}

