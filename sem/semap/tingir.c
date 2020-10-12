#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    fork();

    key_t key = ftok(argv[0], '0');
    int id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    int a = 0, cnt = 0;

    struct sembuf inc, dec;
    inc.sem_num = 0;
    inc.sem_op = 2;
    inc.sem_flg = 0;

    dec.sem_num = 0;
    dec.sem_op = -1;
    dec.sem_flg = 0;

    //union semun set;
    //set.val = 
   // semctl(id, 0, SETVAL, set);

    if (id == -1) {
        id = semget(key, 1, 0);
      /*  while (1) {
            semop(id, &dec, 1);
            printf("%d\n", getpid());
            semop(id, &inc, 1);
        } */
    }
    while (1) {
        semop(id, &dec, 1);
        printf("%d\n", cnt += 2);
        semop(id, &dec, 1);
    }
    
    return 0;
}
