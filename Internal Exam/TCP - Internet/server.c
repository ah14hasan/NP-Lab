// server.c : TCP server using Internet domain sockets.
// Receives a string from client, reverses it, and sends it back.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>   // For sockaddr_in, inet_ntoa, htons, htonl
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 256

// Helper: reverse a string in-place
void reverse_string(char *str) {
    int i = 0, j = strlen(str) - 1;
    char tmp;
    while (i < j) {
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Create a TCP socket (IPv4, stream)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow quick reuse of the port if needed
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Zero out and set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    server_addr.sin_port = htons(PORT);       // Convert port to network byte order

    // Bind socket to IP/port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: listening on port %d...\n", PORT);

    client_len = sizeof(client_addr);
    // Accept a single client connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: got connection from %s:%d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Receive message from client
    bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Server: received \"%s\"\n", buffer);

        // Reverse and send back
        reverse_string(buffer);
        printf("Server: sending reversed \"%s\"\n", buffer);

        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("send");
        }
    } else if (bytes_read == -1) {
        perror("recv");
    } else {
        printf("Server: client closed connection without sending data.\n");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
