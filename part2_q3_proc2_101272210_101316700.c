/**
 * SYSC4001 Assignment 2 Question 3
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(void) {
    int counter = 0;
    int cycle = 0;
    pid_t parent = getppid();
    printf("child started PID: %d, parent PID: %d\n", getpid(), parent);
    while (counter > -500) {
        if (counter % 3 == 0) {
            printf("P2 Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
        } else {
            printf("P2 Cycle number: %d\n", cycle);
        }
        counter--;
        cycle++;
        fflush(stdout);
        usleep(100000);
    }
    printf("\nchild reached %d exiting\n", counter);
    return 0;
}