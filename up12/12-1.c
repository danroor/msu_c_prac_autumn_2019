#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

enum { RIGHTS = S_IRUSR | S_IWUSR };

int
main(int argc, char *argv[])
{
    int nproc, max, num, val, sent;
    key_t key;
    pid_t pid;
    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &max);
    
    int shmid = shmget(key, 2 * sizeof(val), IPC_CREAT | RIGHTS);
    int semid = semget(key, nproc, IPC_CREAT | RIGHTS);
    if (shmid < 0 || semid < 0) {
        return 1;
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } set;
    memset(&set, 0, sizeof(set));
    set.array = calloc(nproc, sizeof(*set.array));
    if (set.array == NULL) {
        return 1;
    }

    if (semctl(semid, nproc, SETALL, set) < 0) {
        return 1;
    }
    free(set.array);

    struct sembuf inc, dec;
    memset(&inc, 0, sizeof(inc));
    memset(&dec, 0, sizeof(dec));
    dec.sem_op = -1;
    inc.sem_op = 1;
    
    for (int i = 0; i < nproc; ++i) {
        pid = fork();
        if (pid == -1) {
            int *mem = shmat(shmid, NULL, 0);
            mem[1] = -1;
            for (int j = 0; j < i; ++j) {
                inc.sem_num = j;
                semop(semid, &inc, 1);
                wait(NULL);
            }

            shmctl(shmid, IPC_RMID, NULL);
            semctl(semid, 0, IPC_RMID, set);
            return 1;
        }
        if (!pid) {
            num = i;
            break;
        }
    }
    
    int *mem = shmat(shmid, NULL, 0);
    
    if (pid) {
        //father
        mem[0] = 0;
        mem[1] = 0;
        inc.sem_num = 0;
        semop(semid, &inc, 1); //unblock proc #1
        
        for (int i = 0; i < nproc; ++i) {
            wait(NULL);
        }
        
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, set);
        return 0;
    }

    dec.sem_num = num;
    while (1) {
        semop(semid, &dec, 1);
        val = mem[0];
        sent = mem[1];
        if (sent < 0) {
            break;   
        }

        printf("%d %d %d\n", num + 1, val, sent);
        fflush(stdout);

        if (val == max) {
            mem[1] = -1;
            for (int i = 0; i < nproc; ++i) {
                inc.sem_num = i;
                semop(semid, &inc, 1);
            }
            break;
        }

        val++;
        mem[0] = val;
        mem[1] = num + 1;
        val %= nproc;
        val *= val;
        val %= nproc;
        inc.sem_num = (val * val) % nproc; //unblock the next process
        semop(semid, &inc, 1);
    }

    return 0;
}
