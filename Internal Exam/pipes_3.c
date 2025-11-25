#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>

int main()
{
    int fd[2], k1, k2;
    char buffer[512];
    char *m = "Welcome to NP Lab";
    k1 = pipe(fd);
    if (k1 == -1) {
        printf("pipe creation failed");
        exit(1);
    }

    k2 = fork();
    if (k2 == -1) {
        printf("Process creation failed");
        exit(2);
    }

    // Parent's context (writer)
    if (k2 != 0) {
        close(fd[0]); // Close the reading end in parent
        write(fd[1], m, strlen(m) + 1); // Write to pipe
        close(fd[1]); // Close the writing end
    }
    // Child's context (reader)
    else {
        close(fd[1]); // Close the writing end in child
        read(fd[0], buffer, sizeof(buffer)); // Read from pipe
        printf("Message received from the parent = %s\n", buffer);
        close(fd[0]); // Close the reading end
    }
}
