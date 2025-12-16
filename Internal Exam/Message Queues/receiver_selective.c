// receiver.c : Selectively receive a message (by type) from the message queue created by sender.c
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
    long desired_type = 2;   // Change this to 1, 2, or 3 depending on which message you want

    // Use the same key as in sender.c
    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Access existing message queue
    msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Selective receive: msgtyp > 0 → first message of that specific type is received [web:122][web:136]
    if (msgrcv(msqid, &message, sizeof(message.mtext), desired_type, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Selectively received message of type %ld: \"%s\"\n",
           message.mtype, message.mtext);

    return 0;
}
