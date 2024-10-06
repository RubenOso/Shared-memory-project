# Shared-memory-project
@readme

README.md
md

# Shared Memory Process Synchronization Project

## Overview
This project demonstrates the use of shared memory and process synchronization in C. Four processes are created, and each increments a shared variable to a specified range. The parent process prints the final results of all processes before allowing any of the child processes to exit, ensuring the output is displayed in a specific order.

### Source File
- **Directory**: `sharedmemory/`
- **Source File**: `main.c`

## Compilation and Execution

### Steps to Compile and Run the Program

1. **Navigate to the directory**:
   ```bash
   cd sharedmemory
Compile the source file: To compile the main.c file using gcc, use the following command:

bash

gcc -o shared_memory main.c -lpthread
This will compile the source code into an executable named shared_memory.

Run the program: To run the compiled program, use the following command:

bash

./shared_memory
Expected Output: The program will print the results of all processes in order (Process 1 to Process 4) before any child process exits. After that, it will print the process IDs of the children as they exit. The output should look like the following:

bash

From Process 1: counter = 100000.
From Process 2: counter = 200000.
From Process 3: counter = 300000.
From Process 4: counter = 500000.
Child with ID: 2412 has just exited.
Child with ID: 2411 has just exited.
Child with ID: 2413 has just exited.
Child with ID: 2415 has just exited.
End of Simulation.
Detailed Explanation of the Program
Shared Memory:

The program uses shmget() and shmat() system calls to create and attach a shared memory segment for storing the results from all child processes.
Process Creation:

Four child processes are created using the fork() system call. Each child process performs a series of increments to its respective counter and stores the final result in the shared memory.
Synchronization:

Each child process sets a "ready" flag when it has completed its work, and the parent waits for all child processes to finish their increments before printing the results. After printing, the child processes are allowed to exit in sequence.
Parent and Child Coordination:

The parent process uses the waitpid() function to ensure that it knows when each child process exits and prints a message when the process finishes.
Mutex Lock:

The program uses a pthread_mutex_t lock to synchronize access to the shared memory, ensuring that multiple processes do not interfere with each other while modifying the shared total variable
