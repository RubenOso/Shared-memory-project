/*
 * Shared Memory Process Synchronization Project
 * Author: Ruben Osornio
 * Username: cs096@cs096
 *
 * This program demonstrates the use of shared memory and process synchronization
 * in a multi-process environment using POSIX shared memory and fork().
 * Four child processes are created to increment a shared variable by different
 * ranges, and the parent ensures proper synchronization and outputs the results
 * in a controlled order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>

// Define a mutex lock for synchronization
pthread_mutex_t lock;

// Define a structure for shared memory to hold process results and ready flags
typedef struct {
    int results[4];  // To store the final results from each process
    int ready[4];    // To indicate when each process has finished its work
} shared_data_t;

/*
 * Function for the first process (Process 1) to increment its counter by 100,000.
 * The result is stored in the shared memory's results array, and the ready flag
 * is set to 1 to indicate that the process has completed its work.
 */
void process1(shared_data_t *data) {
    int counter = 0;
    for (int i = 0; i < 100000; i++) {
        counter++;
    }
    data->results[0] = counter;  // Store the result for Process 1
    data->ready[0] = 1;          // Mark Process 1 as ready
    while (data->ready[0] == 1); // Wait until the parent prints before exiting
}

/*
 * Function for the second process (Process 2) to increment its counter by 200,000.
 * The result is stored in the shared memory's results array, and the ready flag
 * is set to 1 to indicate that the process has completed its work.
 */
void process2(shared_data_t *data) {
    int counter = 0;
    for (int i = 0; i < 200000; i++) {
        counter++;
    }
    data->results[1] = counter;  // Store the result for Process 2
    data->ready[1] = 1;          // Mark Process 2 as ready
    while (data->ready[1] == 1); // Wait until the parent prints before exiting
}

/*
 * Function for the third process (Process 3) to increment its counter by 300,000.
 * The result is stored in the shared memory's results array, and the ready flag
 * is set to 1 to indicate that the process has completed its work.
 */
void process3(shared_data_t *data) {
    int counter = 0;
    for (int i = 0; i < 300000; i++) {
        counter++;
    }
    data->results[2] = counter;  // Store the result for Process 3
    data->ready[2] = 1;          // Mark Process 3 as ready
    while (data->ready[2] == 1); // Wait until the parent prints before exiting
}

/*
 * Function for the fourth process (Process 4) to increment its counter by 500,000.
 * The result is stored in the shared memory's results array, and the ready flag
 * is set to 1 to indicate that the process has completed its work.
 */
void process4(shared_data_t *data) {
    int counter = 0;
    for (int i = 0; i < 500000; i++) {
        counter++;
    }
    data->results[3] = counter;  // Store the result for Process 4
    data->ready[3] = 1;          // Mark Process 4 as ready
    while (data->ready[3] == 1); // Wait until the parent prints before exiting
}

int main() {
    // Initialize the mutex lock for synchronization
    pthread_mutex_init(&lock, NULL);

    // Create shared memory for storing process results and ready flags
    int shm_id = shmget(IPC_PRIVATE, sizeof(shared_data_t), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");  // Handle shared memory allocation error
        exit(1);
    }

    // Attach the shared memory to the parent process
    shared_data_t *data = (shared_data_t *)shmat(shm_id, NULL, 0);

    // Initialize the results and ready flags to 0
    for (int i = 0; i < 4; i++) {
        data->results[i] = 0;
        data->ready[i] = 0;
    }

    // Array to store process IDs
    pid_t pids[4];

    // Create the first child process for Process 1
    if ((pids[0] = fork()) == 0) {
        process1(data);  // Call Process 1 function
        exit(0);         // Child exits after completing its work
    }

    // Create the second child process for Process 2
    if ((pids[1] = fork()) == 0) {
        process2(data);  // Call Process 2 function
        exit(0);         // Child exits after completing its work
    }

    // Create the third child process for Process 3
    if ((pids[2] = fork()) == 0) {
        process3(data);  // Call Process 3 function
        exit(0);         // Child exits after completing its work
    }

    // Create the fourth child process for Process 4
    if ((pids[3] = fork()) == 0) {
        process4(data);  // Call Process 4 function
        exit(0);         // Child exits after completing its work
    }

    // Wait until all processes have set their results and ready flags
    for (int i = 0; i < 4; i++) {
        while (data->ready[i] == 0); // Wait until each process is ready
    }

    // Print the results in the correct order (Process 1 to Process 4)
    printf("From Process 1: counter = %d.\n", data->results[0]);
    printf("From Process 2: counter = %d.\n", data->results[1]);
    printf("From Process 3: counter = %d.\n", data->results[2]);
    printf("From Process 4: counter = %d.\n", data->results[3]);

    // Allow child processes to exit by resetting the ready flags
    for (int i = 0; i < 4; i++) {
        data->ready[i] = 0;
    }

    // Parent process waits for each child process to finish
    for (int i = 0; i < 4; i++) {
        waitpid(pids[i], NULL, 0);  // Wait for each child process to exit
        printf("Child with ID: %d has just exited.\n", pids[i]);
    }

    // Detach from shared memory and release it after use
    shmdt(data);
    shmctl(shm_id, IPC_RMID, NULL);  // Remove the shared memory segment

    // Destroy the mutex lock before exiting
    pthread_mutex_destroy(&lock);

    // Print end of simulation message
    printf("End of Simulation.\n");

    return 0;
}

