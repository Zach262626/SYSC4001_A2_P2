/**
 * SYSC4001 Assignment 2 Question 1
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
    int counter;
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        counter = 0;
        printf("Child started PID: %d\n", getpid());
        while(1) {
            printf("Child Counter: %d\n", counter);
            counter++;
            sleep(1);
        }
    } else {
        counter = 0;
        printf("Parent PID: %d created child PID: %d\n", getpid(), pid);
        while(1) {
            printf("Parent Counter: %d\n", counter);
            counter++;
            sleep(1);
        }
    }

    return 0;
}