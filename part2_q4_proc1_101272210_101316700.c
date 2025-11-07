/**
 * SYSC4001 Assignment 2 Question 4
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define SHM_KEY 1234

int main() {
    int shmid;
    int *shared_mem;   // kept your original variable names
    int *multiple;
    int *counter;

    // create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(int) * 2, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // attach to shared memory
    int *shared  = (int *) shmat(shmid, NULL, 0);
    if (shared == (int *) -1) {
        perror("shmat failed");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    shared[0] = 3; // multiple
    shared[1] = 0; // counter

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    } else if (pid == 0) {
        char shmid_str[32];
        snprintf(shmid_str, sizeof(shmid_str), "%d", shmid);
        char *argv[] = {"./q4_proc2", shmid_str, NULL};
        execv(argv[0], argv);
        perror("execv failed");
        return 1;
    } else {
        int cycle = 1;
        printf("parent PID: %d created child PID: %d\n", getpid(), pid);

        while (1) {
            int multiple = shared[0];
            int counter = shared[1];
            if (counter > 500) {
                printf("P1 shared counter %d > 500, exiting\n", counter);
                break;
            }
            if (counter % 3 == 0) {
                printf("P1 Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
            } else {
                printf("P1 Cycle number: %d\n", cycle);
            }
            shared[1] = counter + 1;
            cycle++;
            fflush(stdout);
            usleep(100000);
        }

        waitpid(pid, NULL, 0);
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        printf("parent exiting.\n");
    }

    return 0;
}