/* udp_server.c : simple UDP echo server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    ssize_t n;

    /* Create UDP socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Clear and fill server address structure */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;    /* listen on all interfaces */
    servaddr.sin_port        = htons(PORT);

    /* Bind the socket to the given IP and port */
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    len = sizeof(cliaddr);

    while (1) {
        /* Receive datagram from client */
        n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                     (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[n] = '\0';  /* null-terminate the received data */
        printf("Client [%s:%d] says: %s\n",
               inet_ntoa(cliaddr.sin_addr),
               ntohs(cliaddr.sin_port),
               buffer);

        /* Echo the same data back to client */
        if (sendto(sockfd, buffer, n, 0,
                   (struct sockaddr *)&cliaddr, len) < 0) {
            perror("sendto failed");
        }
    }

    /* Not reached in this example */
    close(sockfd);
    return 0;
}
