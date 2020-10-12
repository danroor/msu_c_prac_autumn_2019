#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } sas;
    sas.val = 1;

    pid_t pid = fork();

    key_t key = ftok("/home/danroor/1", '0');
    int id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    int a = 0, max = 100,
        cnt = pid ? 1 : 2;

    if (id == -1) {
        id = semget(key, 1, 0);
    }

    struct sembuf inc, dec;
    inc.sem_num = 0;
    inc.sem_op = 1;
    inc.sem_flg = 0;

    dec.sem_num = 0;
    dec.sem_op = -1;
    dec.sem_flg = 0;

    if (pid) semctl(id, 0, SETVAL, sas);

    while (cnt < max) {
        semop(id, &dec, 1);
        printf("%d\n", cnt);
        cnt += 2;
        semop(id, &inc, 1);
    }
    
    return 0;
}
