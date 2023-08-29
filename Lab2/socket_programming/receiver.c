#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const char *receiver_ip = "127.0.0.1";  // Hardcoded server IP address
    int receiver_port = 12345;             // Hardcoded server port

    // Create a socket using IPv4 addressing and UDP protocol
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation error");
        return 1;
    }

    // Define the receiver address structure
    struct sockaddr_in receiver_addr;
    receiver_addr.sin_family = AF_INET;            // IPv4 address family
    receiver_addr.sin_port = htons(receiver_port); // Convert port to network byte order
    receiver_addr.sin_addr.s_addr = inet_addr(receiver_ip); // Convert IP to network byte order

    // Bind the socket to the receiver address
    if (bind(sockfd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr)) < 0) {
        perror("Binding error");
        return 1;
    }

    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer before receiving
        recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);  // Receive message

        printf("Received: %s\n", buffer);

        // Check if the received message is "exit" to exit the loop
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}

