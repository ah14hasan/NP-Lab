// IPC using message queues (receiver's program)
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct msg_buffer {
long msg_type;
char msg_text[MAX_TEXT];
};

int main() {
key_t key = 1234;
int msgid;
struct msg_buffer message;

 // Access the message queue
msgid = msgget(key, 0666 | IPC_CREAT);
if (msgid == -1) {
perror("msgget");
exit(1);
}

 // Receive the message
if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
perror("msgrcv");
exit(2);
}

printf("Message received: %s", message.msg_text);

 // Destroy the message queue
msgctl(msgid, IPC_RMID, NULL);
}
