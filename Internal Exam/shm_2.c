#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 1024      // Number of bytes in shared memory
#define MSG "Hello from Parent to Child using Shared Memory!\n"

int main() {
    int shmid;
    char *shared_mem;

    // Create a shared memory segment; returns shared memory identifier
    // IPC_PRIVATE means a unique segment; 0666 is permission bits
    shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");   // Error creating shared memory
        exit(1);
    }

    // Fork the process: creates parent and child
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");     // Error creating process
        exit(2);
    } 
    else if (pid > 0) {
        // --- Parent process section ---
        // Attach shared memory segment; returns pointer to start of segment
        shared_mem = shmat(shmid, NULL, 0);
        if (shared_mem == (void*)-1) {
            perror("shmat (parent)"); // Error attaching shared memory
            exit(3);
        }

        // Write message to shared memory
        strcpy(shared_mem, MSG); // Copy string to shared memory
        printf("Parent: Message written to shared memory.\n");

        // Wait for child process to finish reading
        wait(NULL);

        // Detach shared memory segment from parent's address space
        shmdt(shared_mem);

        // Remove (destroy) the shared memory segment after communication
        shmctl(shmid, IPC_RMID, NULL);
    } 
    else {
        // --- Child process section ---
        // Sleep for 1 second to make sure parent writes first
        sleep(1);

        // Attach to the same shared memory segment as parent
        shared_mem = shmat(shmid, NULL, 0);
        if (shared_mem == (void*)-1) {
            perror("shmat (child)");  // Error attaching shared memory
            exit(4);
        }

        // Read message from shared memory and print it
        printf("Child: Message read from shared memory: %s", shared_mem);

        // Detach shared memory segment from child's address space
        shmdt(shared_mem);

        exit(0); // Child process ends
    }
}
