// client.c : TCP client using Internet domain sockets.
// Sends a string to the server and prints the reversed reply.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>   // For sockaddr_in, inet_pton, htons
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"   // Localhost
#define PORT 8080
#define BUFFER_SIZE 256

int main(void) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Create a TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;          // IPv4
    server_addr.sin_port = htons(PORT);        // Port in network byte order

    // Convert IP string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // Get input from user
    printf("Client: enter a message: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Client: error reading input\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // Strip trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';

    // Send to server
    if (send(sock_fd, buffer, strlen(buffer), 0) == -1) {
        perror("send");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // Receive reversed message from server
    bytes_read = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Client: received reversed message: \"%s\"\n", buffer);
    } else if (bytes_read == -1) {
        perror("recv");
    } else {
        printf("Client: server closed connection.\n");
    }

    close(sock_fd);
    return 0;
}
