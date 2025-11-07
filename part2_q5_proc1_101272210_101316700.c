/**
 * SYSC4001 Assignment 2 Question 5
 * Zachary Gallant 101272210
 * Joseph Dereje 101316700
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define SHM_KEY 1234

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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

int main(void) {
    int shmid;
    int semid;
    int *shared;

    // create shared memory segment
    shmid = shmget(SHM_KEY, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // attach to shared memory
    shared = (int *) shmat(shmid, NULL, 0);
    if (shared == (int *) -1) {
        perror("shmat failed");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    //create a semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget failed");
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }
    //init semaphore
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL failed");
        semctl(semid, 0, IPC_RMID);
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }
    //init shared memory
    sem_op(semid, -1);
    shared[0] = 3;
    shared[1] = 0; 
    sem_op(semid, 1);

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        semctl(semid, 0, IPC_RMID);
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    } else if (pid == 0) {
        char shmid_str[32], semid_str[32];
        snprintf(shmid_str, sizeof(shmid_str), "%d", shmid);
        snprintf(semid_str, sizeof(semid_str), "%d", semid);
        char *argv[] = {"./q5_proc2", shmid_str, semid_str, NULL};
        execv(argv[0], argv);
        perror("execv failed");
        return 1;
    } else {
        int cycle = 1;
        printf("parent PID: %d created child PID: %d\n", getpid(), pid);

        while (1) {
            sem_op(semid, -1);
            int multiple = shared[0];
            int counter = shared[1];

            if (counter > 500) {
                printf("P1 shared counter %d > 500, exiting\n", counter);
                sem_op(semid, 1);
                break;
            }

            if (counter % multiple == 0) {
                printf("P1 Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
            } else {
                printf("P1 Cycle number: %d\n", cycle);
            }

            shared[1] = counter + 1;
            sem_op(semid, 1);

            cycle++;
            fflush(stdout);
            usleep(100000);
        }

        waitpid(pid, NULL, 0);
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        printf("parent exiting.\n");
    }

    return 0;
}
