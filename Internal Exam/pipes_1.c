// Write a C program that illustrates how to execute two commands
// concurrently with a command pipe. Ex:- ls –l | sort
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/ipc.h>
#include<unistd.h>
int main()
{
int fd[2], k1, k2;
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
 dup2(fd[1],1); // output redirection - output goes to pipe instead of screen
 close(fd[1]);
 execl("/bin/ls", "ls", "-l", NULL);
 printf("Error"); // if execl fails
}
else // Parent's context (reader)
{
close(fd[1]);
dup2(fd[0],0); // input redirection
close (fd[0]);
execl("/bin/sort", "sort", NULL);
printf("Error");
}
}
