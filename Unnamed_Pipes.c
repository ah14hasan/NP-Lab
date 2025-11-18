// IPC Using Unnamed Pipes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/ipc.h>
#include<unistd.h>
int main()
{
int fd[2], k1, k2;
char buffer[512];
char *m = "Welcome to NP Lab";
k1 = pipe(fd);
if(k1 == -1)
{
 printf("pipe creation failed");
exit(1);
}

k2 = fork();
if (k2 == -1)
{
 printf("Process creation failed");
exit(2);
}

 // Child's Context
 // Assume the child is writer
 // Use fd[1] and close fd[0]
if(k2==0)
{
 close(fd[0]);
write(fd[1], m, strlen(m)+1);
close(fd[1]);
}
else // Parent's context (reader)
{
 close(fd[1]);
read(fd[0], buffer, sizeof(buffer));
printf("Message received from the child = %s\n", buffer);
close (fd[0]);
}
}
