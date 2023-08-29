#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Check the number of command-line arguments
    if (argc != 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[250];

    // Create a socket using IPv4 and TCP
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_fd);
        return 1;
    }

    printf("Enter the request message to the server: ");
    fgets(buffer, sizeof(buffer), stdin);
    // Send request message to the server
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive response from the server
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("Received response message from server: %s", buffer);

    // Close the client socket
    close(client_fd);
    return 0;
}

