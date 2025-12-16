// delete_queue.c : Delete the message queue created by sender.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void) {
    key_t key;
    int msqid;

    // Use the same key generation as in sender.c
    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the existing message queue
    msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Delete the message queue using msgctl with IPC_RMID [web:144][web:147]
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl - IPC_RMID");
        exit(EXIT_FAILURE);
    }

    printf("Message queue deleted successfully.\n");
    return 0;
}

/*You can verify before/after with ipcs -q to see the queue entry disappear once delete_queue runs.*/
