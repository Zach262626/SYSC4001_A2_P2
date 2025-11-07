/**
 * SYSC4001 Assignment 2 Question 2
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        char *argv[] = {"./q2_proc2", NULL};
        execv(argv[0], argv);
        perror("execv");
        return 1;
    } else {
        long counter = 0;
        int cycle = 0;
        printf("process1 PID: %d created process2 PID: %d\n", getpid(), pid);
        while (1) {
            if (counter % 3 == 0) {
                printf("P1 Cycle number: %d - %ld is a multiple of 3\n", cycle, counter);
            } else {
                printf("P1 Cycle number: %d\n", cycle);
            }
            counter++;
            cycle++;
            fflush(stdout);
            usleep(300000);
        }
    }

    return 0;
}