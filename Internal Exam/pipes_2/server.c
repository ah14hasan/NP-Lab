// IPC using named pipes (sender's program)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/myfifo"

int main() {
int fd;
char arr[100];

 // Create the named pipe (FIFO) if it does not exist
mkfifo(FIFO_PATH, 0666);

printf("Enter a message to send: ");
fgets(arr, sizeof(arr), stdin);

 // Open FIFO for writing
fd = open(FIFO_PATH, O_WRONLY);
write(fd, arr, strlen(arr)+1);
close(fd);

printf("Message sent.\n");
}
