// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int server_fd, client_fds[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    fd_set read_fds, master_fds;

    int port = atoi(argv[1]);
    int max_fd, activity, i, client_count = 0;
    socklen_t addr_len;

    char buffer[BUFFER_SIZE];

    for (i = 0; i < MAX_CLIENTS; i++) {
        client_fds[i] = 0;
    }

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server listening on port %d...\n", port);

    FD_ZERO(&master_fds);
    FD_SET(server_fd, &master_fds);
    max_fd = server_fd;

    while (1) {
        read_fds = master_fds;
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Select error");
            exit(1);
        }

        for (i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == server_fd) {
                    // New connection
                    int client_fd, addr_len = sizeof(client_addr);
                    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
                        perror("Accept error");
                        exit(1);
                    }

                    printf("Accepted connection from client with IP Addr : %s and port Number : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    FD_SET(client_fd, &master_fds);
                    if (client_fd > max_fd) {
                        max_fd = client_fd;
                    }

                    client_fds[client_count++] = client_fd;
                } else {
                    // Handle data from a client
                    int valread;
                    if ((valread = read(i, buffer, BUFFER_SIZE)) <= 0) {
                        // Connection closed or error
                        getpeername(i, (struct sockaddr *)&client_addr, &addr_len);
                        printf("Client %s on port %d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                        close(i);
                        FD_CLR(i, &master_fds);

                        // Remove from the client_fds array
                        for (int j = 0; j < client_count; j++) {
                            if (client_fds[j] == i) {
                                for (int k = j; k < client_count - 1; k++) {
                                    client_fds[k] = client_fds[k + 1];
                                }
                                client_count--;
                            }
                        }
                    } else {
                        // Broadcast the message to all other clients
                        buffer[valread] = '\0';  // Null-terminate the received message
                        printf("Message from client %s on port %d : %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
                        for (int j = 0; j < client_count; j++) {
                            if (client_fds[j] != i) {
                                send(client_fds[j], buffer, valread, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
