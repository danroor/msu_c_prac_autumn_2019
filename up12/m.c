#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    ARG1 = 1,
    ARG2 = 2,
    ARG3 = 3,
    ARG4 = 4,
    ARG5 = 5,
    NUM = 10,
    DN = -1,
    UP = 1,
    RIGHTS = 0600
};

int count, iter_count;

void
operation(int *data, int ind1, int ind2, int value)
{
    if (ind1 != ind2) {
        int tmp1 = data[ind1] - value;
        int tmp2 = data[ind2] + value;

        data[ind1] = tmp1;
        data[ind2] = tmp2;
    }
}

void work(int semid, int *buf)
{
    while (1) {
        if (iter_count <= 0) {
            _exit(0);
        }
        int j1 = rand() % count;
        int j2 = rand() % count;
        int val = rand() % NUM;
        struct sembuf ops[ARG2];
        ops[0].sem_num = j1;
        ops[0].sem_op = DN;
        ops[1].sem_num = j2;
        ops[1].sem_op = DN;
        if (j1 == j2) {
            semop(semid, ops, 1);
        } else {
            semop(semid, ops, ARG2);
        }
        iter_count--;
        operation(buf, j1, j2, val);
        ops[0].sem_num = j1;
        ops[0].sem_op = UP;
        ops[1].sem_num = j2;
        ops[1].sem_op = UP;
        if (j1 == j2) {
            semop(semid, ops, 1);
        } else {
            semop(semid, ops, ARG2);
        }
    }
}

int main(int argc, char *argv[])
{
    int nproc;
    key_t key;
    sscanf(argv[ARG1], "%d", &count);
    sscanf(argv[ARG2], "%d", &key);
    sscanf(argv[ARG3], "%d", &nproc);
    sscanf(argv[ARG4], "%d", &iter_count);
    int *buf;
    int shmid = shmget(key, count * sizeof(*buf), IPC_CREAT | IPC_EXCL | RIGHTS);
    buf = shmat(shmid, NULL, 0);
    for (int i = 0; i < count; i++) {
        scanf("%d", &buf[i]);
    }
    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | RIGHTS);
    for (int i = 0; i < count; i++) {
        semctl(semid, i, SETVAL, 1);
    }
    for (int i = 0; i < nproc; i++) {
        unsigned int seed;
        sscanf(argv[ARG5 + i], "%u", &seed);
        if (fork() == 0) {
            srand(seed);
            work(semid, buf);
            exit(0);
        }
    }
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
    for (int i = 0; i < count; i++) {
        printf("%d\n", buf[i]);;
    }
    fflush(stdout);
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, 0, IPC_RMID);
    return 0;
}
