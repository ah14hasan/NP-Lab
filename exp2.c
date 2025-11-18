#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>

#define SHM_KEY 3456 // Key for shared memory segment

// Structure for shared data: an integer and a semaphore
typedef struct {
    int value;      // This variable will be shared and incremented
    sem_t sem;      // POSIX unnamed semaphore for synchronization
} shared_data;

int main() {
    // Create a shared memory segment big enough for our structure
    int shmid = shmget(SHM_KEY, sizeof(shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment to the process' address space
    shared_data* data = (shared_data*) shmat(shmid, NULL, 0);
    if (data == (void*)-1) {
        perror("shmat");
        exit(2);
    }

    // Initialize the shared integer and semaphore
    data->value = 0;
    // sem_init: pshared=1 makes the semaphore visible to all processes with access to this memory
    if (sem_init(&(data->sem), 1, 1) != 0) {
        perror("sem_init");
        exit(3);
    }

    // Create the first child process
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // In child 1: increment the shared variable 1000 times
        for (int i = 0; i < 1000; ++i) {
            sem_wait(&data->sem);   // Enter critical section
            data->value += 1;       // Critical operation
            sem_post(&data->sem);   // Exit critical section
        }
        shmdt(data); // Detach shared memory segment
        exit(0);
    }

    // Create the second child process
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // In child 2: increment the shared variable 1000 times
        for (int i = 0; i < 1000; ++i) {
            sem_wait(&data->sem);   // Enter critical section
            data->value += 1;       // Critical operation
            sem_post(&data->sem);   // Exit critical section
        }
        shmdt(data); // Detach shared memory segment
        exit(0);
    }

    // In parent: wait for both children to finish
    wait(NULL);
    wait(NULL);

    // Display the final value of the shared variable
    printf("Final value in shared memory: %d\n", data->value);

    // Cleanup: destroy the semaphore and remove the shared memory segment
    sem_destroy(&(data->sem));
    shmdt(data);               // Detach from shared memory
    shmctl(shmid, IPC_RMID, NULL); // Mark the shared segment for deletion
}
