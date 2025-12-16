// client.c : Unix domain socket client (connection-oriented, local)
// Sends a string to the server and prints the reversed reply.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "uds_socket"
#define BUFFER_SIZE 256

int main(void) {
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Create a Unix domain stream socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Connect to the server socket
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Read a line from stdin to send
    printf("Client: enter a message: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Client: error reading input\n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Remove trailing newline, if any
    buffer[strcspn(buffer, "\n")] = '\0';

    // Send the message to the server
    if (write(client_fd, buffer, strlen(buffer)) == -1) {
        perror("write");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Receive the reversed message from the server
    bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Client: received reversed message: \"%s\"\n", buffer);
    } else if (bytes_read == -1) {
        perror("read");
    } else {
        printf("Client: server closed connection.\n");
    }

    close(client_fd);
    return 0;
}
