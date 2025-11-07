/**
 * SYSC4001 Assignment 2 Question 3
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        char *argv[] = {"./q3_proc2", NULL};
        execv(argv[0], argv);
        perror("execv");
        return 1;
        } else {
        int counter = 1;
        int cycle = 1;
        printf("parent PID: %d created child PID: %d\n", getpid(), pid);
        
        while (waitpid(pid, NULL, WNOHANG) == 0) {
            if (counter % 3 == 0) {
            printf("P1 Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
            } else {
            printf("P1 Cycle number: %d\n", cycle);
            }
            counter++;
            cycle++;
            fflush(stdout);
            usleep(100000);
        }
        
        printf("child exited, parent now exiting.\n");
        return 0;
        }

    return 0;
}