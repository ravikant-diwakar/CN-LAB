#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void send_file(int client_socket, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(client_socket, buffer, bytesRead, 0) == -1) {
            perror("Error sending file");
            break;
        }
    }

    fclose(file);
    printf("Done sending file %s\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening");
        return 1;
    }

    printf("Listening on port %d\n", port);

    while (1) {
        printf("Waiting for connection from client...\n");
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(".")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_type == DT_REG) {
                    send(client_socket, ent->d_name, strlen(ent->d_name), 0);
                    send(client_socket, "\n", 1, 0);
                }
            }
            closedir(dir);
        } else {
            perror("Error opening directory");
        }

        char requested_file[MAX_BUFFER_SIZE];
        ssize_t bytes_received = recv(client_socket, requested_file, sizeof(requested_file), 0);
        if (bytes_received <= 0) {
            perror("Error receiving filename");
            close(client_socket);
            continue;
        }
        requested_file[bytes_received] = '\0';

        printf("Trying to open the file: %s\n", requested_file);
        send_file(client_socket, requested_file);

        close(client_socket);
        printf("Waiting for connection from client...\n");
    }

    close(server_socket);

    return 0;
}
