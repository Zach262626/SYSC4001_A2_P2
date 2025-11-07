/**
 * SYSC4001 Assignment 2 Question 2
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {
    long counter = 0;
    int cycle = 0;
    printf("process2 started PID: %d\n", getpid());
    while (1) {
        if ((counter % 3) == 0) {
            printf("P2 Cycle number: %d - %ld is a multiple of 3\n", cycle, counter);
        } else {
            printf("P2 Cycle number: %d\n", cycle);
        }
        counter--;
        cycle++;
        fflush(stdout);
        usleep(300000);
    }
    return 0;
}