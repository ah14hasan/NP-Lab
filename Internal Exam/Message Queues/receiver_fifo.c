// receiver.c : Receive messages in FIFO order from the message queue created by sender.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 128

struct msgbuf {
    long mtype;              // Message type (priority)
    char mtext[MAX_TEXT];    // Message text
};

int main(void) {
    key_t key;
    int msqid;
    struct msgbuf message;
    int i;

    // Use the same key as in sender.c (ftok(".", 'A'))
    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the existing message queue (created by sender)
    msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Receive 3 messages in FIFO order (msgtyp = 0 → first message on queue)
    for (i = 0; i < 3; i++) {
        if (msgrcv(msqid, &message, sizeof(message.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("Received %d: \"%s\" (type = %ld)\n", i + 1, message.mtext, message.mtype);
    }

    // Optionally remove the queue after use
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
