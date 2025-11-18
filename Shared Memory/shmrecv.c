// IPC using shared memory (reader's program)
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

 // Locate shared memory segment
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

printf("Message read from shared memory: %s\n", (char*)shared_mem);

 // Detach from shared memory
shmdt(shared_mem);

 // Remove the shared memory segment
shmctl(shmid, IPC_RMID, NULL);
}
