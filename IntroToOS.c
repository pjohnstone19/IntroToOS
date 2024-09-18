#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define NUM_PROCESSES 4  // Adjust this number to run 3 or 4 processes

// Function to be executed by each child process
void child_process_function(int process_number) {
    printf("Child Process %d: Starting (PID: %d)\n", process_number, getpid());
    sleep(10);  // Simulate some work
    printf("Child Process %d: Finishing (PID: %d)\n", process_number, getpid());
    exit(0);  // Exit the process
}

int main() {
    pid_t pids[NUM_PROCESSES];  // Array to store child PIDs

    // Create multiple processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pids[i] = fork();  // Fork a new process

        if (pids[i] < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            // Child process
            child_process_function(i + 1);
        } else {
            // Parent process
            printf("Parent Process: Created child process %d with PID %d\n", i + 1, pids[i]);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            printf("Parent Process: Child %d with PID %d exited with status %d\n", i + 1, pids[i], WEXITSTATUS(status));
        }
    }

    // Option to kill a specific process (uncomment to use)
    
    printf("Enter the PID of the process to kill: ");
    pid_t pid_to_kill;
    scanf("%d", &pid_to_kill);
    if (kill(pid_to_kill, SIGKILL) == 0) {
        printf("Successfully killed process with PID %d\n", pid_to_kill);
    } else {
        perror("Failed to kill process");
    }
    

    printf("All processes have finished.\n");
    return 0;
}
