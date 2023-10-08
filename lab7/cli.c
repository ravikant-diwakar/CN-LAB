#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void receive_file(int server_socket, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = recv(server_socket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytesRead, file);
    }

    fclose(file);
    printf("Written file %s-dl successfully\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        return 1;
    }

    char filename[MAX_BUFFER_SIZE];
    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';  // Remove newline character

    send(client_socket, filename, strlen(filename), 0);

    char server_response[MAX_BUFFER_SIZE];
    ssize_t bytes_received;
    
    while ((bytes_received = recv(client_socket, server_response, sizeof(server_response), 0)) > 0) {
        server_response[bytes_received] = '\0';
        printf("bytes received=%ld\n", bytes_received);
    }

    if (bytes_received < 0) {
        perror("Error receiving file");
    } else if (bytes_received == 0) {
        printf("Written file %s-dl successfully\n", filename);
    }

    close(client_socket);

    return 0;
}
