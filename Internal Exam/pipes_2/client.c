// IPC using named pipes (receiver's program)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/myfifo"

int main() {
int fd;
char arr[100];

 // Create the named pipe (FIFO) if it does not exist
mkfifo(FIFO_PATH, 0666);

 // Open FIFO for reading
fd = open(FIFO_PATH, O_RDONLY);
read(fd, arr, sizeof(arr));
printf("Message received: %s\n", arr);
close(fd);
}
