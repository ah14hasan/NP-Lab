// sender.c : Create a message queue and send 3 messages with different priorities
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 128

// User-defined message structure: first field MUST be long (message type/priority)
struct msgbuf {
    long mtype;              // Used here as priority (must be > 0)
    char mtext[MAX_TEXT];    // Message text
};

int main(void) {
    key_t key;
    int msqid;
    struct msgbuf message;

    // Generate a key for the message queue
    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue with read & write permissions: 0666
    msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // --------- First message (priority 1) ----------
    message.mtype = 1;
    strcpy(message.mtext, "First message with priority 1");
    printf("About to send: \"%s\" with type %ld\n", message.mtext, message.mtype);
    if (msgsnd(msqid, &message, strlen(message.mtext) + 1, 0) == -1) {
        perror("msgsnd - 1");
        exit(EXIT_FAILURE);
    }

    // --------- Second message (priority 2) ----------
    message.mtype = 2;
    strcpy(message.mtext, "Second message with priority 2");
    printf("About to send: \"%s\" with type %ld\n", message.mtext, message.mtype);
    if (msgsnd(msqid, &message, strlen(message.mtext) + 1, 0) == -1) {
        perror("msgsnd - 2");
        exit(EXIT_FAILURE);
    }

    // --------- Third message (priority 3) ----------
    message.mtype = 3;
    strcpy(message.mtext, "Third message with priority 3");
    printf("About to send: \"%s\" with type %ld\n", message.mtext, message.mtype);
    if (msgsnd(msqid, &message, strlen(message.mtext) + 1, 0) == -1) {
        perror("msgsnd - 3");
        exit(EXIT_FAILURE);
    }

    printf("Three messages sent to the message queue.\n");
    return 0;
}
