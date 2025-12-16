// server.c : Unix domain socket server (connection-oriented, local)
// Receives a string from client, reverses it, and sends it back.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "uds_socket"
#define BUFFER_SIZE 256

// Helper function to reverse a string in-place (excluding terminating '\0')
void reverse_string(char *str) {
    int i, j;
    char tmp;
    i = 0;
    j = strlen(str) - 1;
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
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Create a Unix domain stream socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Ensure any previous socket file is removed
    unlink(SOCKET_PATH);

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Bind the socket to the given path
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
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

    printf("Server: waiting for a connection on %s ...\n", SOCKET_PATH);

    // Accept a single client connection
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read message from client
    bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the string
        printf("Server: received \"%s\"\n", buffer);

        // Reverse the string
        reverse_string(buffer);
        printf("Server: sending reversed \"%s\"\n", buffer);

        // Send reversed string back to client
        if (write(client_fd, buffer, strlen(buffer)) == -1) {
            perror("write");
        }
    } else if (bytes_read == -1) {
        perror("read");
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);  // Remove the socket file
    return 0;
}
