/* tcp_client.c : simple TCP client */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE    1024

int main(void) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUF_SIZE];
    ssize_t n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type messages (\"exit\" to quit):\n");

    while (1) {
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            break;
        }

        n = recv(sockfd, buffer, BUF_SIZE - 1, 0);
        if (n <= 0) {
            printf("Server closed connection.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Echo: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;
}
