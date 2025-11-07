/**
 * SYSC4001 Assignment 2 Question 5
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>

void sem_op(int semid, short op) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int shmid, semid;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <shmid> <semid>\n", argv[0]);
        exit(1);
    }

    shmid = atoi(argv[1]);
    semid = atoi(argv[2]);

    //attach to shared memory
    int *shared = (int *) shmat(shmid, NULL, 0);
    if (shared == (int *) -1) {
        perror("Child: shmat failed");
        exit(1);
    }

    printf("child PID %d attached wa ting for counter > 100 to start.\n", getpid());

    while (1) {
        sem_op(semid, -1);
        int counter = shared[1];
        sem_op(semid, 1);
        if (counter > 100) break;
        usleep(100000);
    }

    pid_t parent = getppid();
    printf("child started PID: %d, parent PID: %d\n", getpid(), parent);

    int cycle = 0;
    while (1) {
        sem_op(semid, -1);
        int multiple = shared[0];
        int counter = shared[1];

        if (counter > 500) {
            sem_op(semid, 1);
            break;
        }

        if (counter % multiple == 0) {
            printf("P2 Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
        } else {
            printf("P2 Cycle number: %d\n", cycle);
        }

        shared[1] = counter - 1;
        sem_op(semid, 1);

        cycle++;
        fflush(stdout);
        usleep(150000);
    }
    printf("\nchild exiting\n");
    shmdt(shared);
    return 0;
}
