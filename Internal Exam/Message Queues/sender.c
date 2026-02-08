// sender.c : Create a message queue and send 3 messages with user-given priorities and texts
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 128

struct msgbuf {
    long mtype;              // message type/priority (> 0)
    char mtext[MAX_TEXT];    // message text
};

int main(void) {
    key_t key;
    int msqid;
    struct msgbuf message;
    int i;

    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 3; i++) {
        printf("Enter priority for message %d (positive integer): ", i + 1);
        if (scanf("%ld", &message.mtype) != 1 || message.mtype <= 0) {
            fprintf(stderr, "Invalid priority.\n");
            exit(EXIT_FAILURE);
        }

        // Clear leftover newline from input buffer before using fgets.
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        printf("Enter text for message %d: ", i + 1);
        if (fgets(message.mtext, MAX_TEXT, stdin) == NULL) {
            fprintf(stderr, "Error reading message text.\n");
            exit(EXIT_FAILURE);
        }

        // Remove trailing newline from fgets, if present.
        size_t len = strlen(message.mtext);
        if (len > 0 && message.mtext[len - 1] == '\n') {
            message.mtext[len - 1] = '\0';
        }

        printf("About to send: \"%s\" with type %ld\n", message.mtext, message.mtype);

        if (msgsnd(msqid, &message, strlen(message.mtext) + 1, 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    printf("Three messages sent to the message queue.\n");
    return 0;
}
