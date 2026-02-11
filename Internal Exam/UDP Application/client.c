/* udp_client.c : simple UDP client */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP   "127.0.0.1"  /* change to server IP if remote */
#define SERVER_PORT 8080
#define BUF_SIZE    1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in servaddr;
    socklen_t addr_len;
    ssize_t n;

    /* Create UDP socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Clear and fill server address structure */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        perror("invalid address / address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(servaddr);

    printf("Enter message: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "input error\n");
        close(sockfd);
        return 1;
    }

    /* Remove trailing newline from fgets, if present */
    buffer[strcspn(buffer, "\n")] = '\0';

    /* Send message to server */
    if (sendto(sockfd, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&servaddr, addr_len) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* Receive echo from server */
    n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                 (struct sockaddr *)&servaddr, &addr_len);
    if (n < 0) {
        perror("recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Server replied: %s\n", buffer);

    close(sockfd);
    return 0;
}
