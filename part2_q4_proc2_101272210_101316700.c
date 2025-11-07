/**
 * SYSC4001 Assignment 2 Question 4
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    int shmid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shmid>\n", argv[0]);
        exit(1);
    }

    // get shared memory ID from command line
    shmid = atoi(argv[1]);

    // attach to shared memory
    int *shared = (int *) shmat(shmid, NULL, 0);
    if (shared == (int *) -1) {
        perror("Child: shmat failed");
        exit(1);
    }

    printf("child PID %d attached wa ting for counter > 100 to start.\n", getpid());

    while (shared[1] <= 100) {
        usleep(100000);
    }

    pid_t parent = getppid();
    printf("child started PID: %d, parent PID: %d\n", getpid(), parent);
    int cycle = 0;
    int counter = shared[1];
    while (counter > -500) {
        int multiple = shared[0];
        counter = shared[1];
        if (counter % 3 == 0) {
            printf("P2 Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
        } else {
            printf("P2 Cycle number: %d\n", cycle);
        }
        shared[1] = counter - 1;
        cycle++;
        fflush(stdout);
        usleep(150000);
    }

    printf("\nchild reached %d exiting\n", counter);
    shmdt(shared);
    return 0;
}