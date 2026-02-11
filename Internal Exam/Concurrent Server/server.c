/* concurrent_server.c : simple concurrent TCP echo server using fork() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT     8080
#define BACKLOG  5
#define BUF_SIZE 1024

void handle_client(int client_fd) {
    char buffer[BUF_SIZE];
    ssize_t n;

    while (1) {
        n = recv(client_fd, buffer, BUF_SIZE - 1, 0);
        if (n <= 0) {
            /* connection closed or error */
            break;
        }
        buffer[n] = '\0';
        printf("Child %d received: %s\n", getpid(), buffer);

        /* echo back */
        if (send(client_fd, buffer, n, 0) < 0) {
            perror("send");
            break;
        }

        /* simple exit condition from client */
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(client_fd);
    printf("Child %d done.\n", getpid());
}

int main(void) {
    int listen_fd, client_fd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cli_len;
    pid_t pid;

    /* avoid zombie children: ignore SIGCHLD */
    signal(SIGCHLD, SIG_IGN);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* allow quick reuse of the port */
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(PORT);

    if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, BACKLOG) < 0) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Concurrent TCP server listening on port %d...\n", PORT);

    while (1) {
        cli_len = sizeof(cliaddr);
        client_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &cli_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        printf("New connection from %s:%d\n",
               inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        pid = fork();
        if (pid < 0) {
            perror("fork");
            close(client_fd);
            continue;
        } else if (pid == 0) {  /* child */
            close(listen_fd);   /* child does not need listening socket */
            handle_client(client_fd);
            exit(0);
        } else {               /* parent */
            close(client_fd);   /* parent does not need client socket */
        }
    }

    close(listen_fd);
    return 0;
}
