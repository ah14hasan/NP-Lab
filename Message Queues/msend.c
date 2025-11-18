// IPC using message queues (sender's program)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

 // Create message queue and return identifier
msgid = msgget(key, 0666 | IPC_CREAT);
if (msgid == -1) {
perror("msgget");
exit(1);
}

printf("Enter a message to send: ");
fgets(message.msg_text, MAX_TEXT, stdin);

 // Set message type
message.msg_type = 1;

 // Send the message
if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
perror("msgsnd");
exit(2);
}

printf("Message sent.\n");
}
