// IPC using shared memory (writer's program)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024 // Size of shared memory
#define SHM_KEY 2345  // Shared memory key

int main() {
int shmid;
void *shared_mem;
char buffer[100];

 // Create shared memory segment
shmid = shmget(SHM_KEY, SHM_SIZE, 0666|IPC_CREAT);
if (shmid == -1) {
perror("shmget");
exit(1);
}

 // Attach to the shared memory
shared_mem = shmat(shmid, NULL, 0);
if (shared_mem == (void*)-1) {
perror("shmat");
exit(2);
}

printf("Enter a message to write to shared memory: ");
fgets(buffer, sizeof(buffer), stdin);

 // Copy data to shared memory
strcpy((char*)shared_mem, buffer);

printf("Message written to shared memory.\n");

 // Detach from shared memory
shmdt(shared_mem);
}
