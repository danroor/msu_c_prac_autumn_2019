#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

enum
{
    RIGHTS = S_IRUSR | S_IWUSR,
    MAXRANDVAL = 10,
    SEEDS_IND = 5
};

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
int
main(int argc, char *argv[])
{
    if (argc < SEEDS_IND) {
        return 1;
    }

    int count, nproc, iter_count, num;
    key_t key;
    pid_t pid;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);

    if (argc < SEEDS_IND + nproc) {
        return 1;    
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } set;
    memset(&set, 0, sizeof(set));
    set.array = calloc(count, sizeof(*set.array));
    if (set.array == NULL) {
        return 1;
    }
  
    for (int i = 0; i < count; ++i) {
        set.array[i] = 1;
    }
    
    int semid = semget(key, count, IPC_CREAT | RIGHTS);
    if (semid < 0 || semctl(semid, count, SETALL, set) < 0) {
        return 1;
    }
    free(set.array);

    int shmid = shmget(key, count * sizeof(count), IPC_CREAT | RIGHTS);
    if (shmid < 0) {
        return 1;
    }
    int *mem = shmat(shmid, NULL, 0);
    for (int i = 0; i < count; ++i) {
        scanf("%d", &mem[i]);
    }

    for (int i = 0; i < nproc; ++i) {
        pid = fork();
        if (pid < 0) {
            for (int j = 0; j < i; ++j) {
                wait(NULL);
            }
            return 1;
        }
        if (!pid) {
            num = i;
            break;
        }
    }
    
    if (pid) {
        //father
        for (int i = 0; i < nproc; ++i) {
            wait(NULL);
        }

        for (int i = 0; i < count; ++i) {
            printf("%d\n", mem[i]);
        }
        
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, set);
        return 0;
    }

    //sons
    struct sembuf inc[2], dec[2];
    memset(inc, 0, sizeof(inc));
    memset(dec, 0, sizeof(dec));
    dec[0].sem_op = dec[1].sem_op = -1;
    inc[0].sem_op = inc[1].sem_op = 1;

    int ind1, ind2, val;
    sscanf(argv[num + SEEDS_IND], "%d", &val);
    srand(val);

    for (int i = 0; i < iter_count; ++i) {
        ind1 = rand() % count;
        ind2 = rand() % count;
        val = rand() % MAXRANDVAL;

        dec[0].sem_num = inc[0].sem_num = ind1;
        dec[1].sem_num = inc[1].sem_num = ind2;
        semop(semid, dec, 1 + (ind1 != ind2) ); 
        //if ind1 == ind2, the semaphore must be decreased only once

        operation(mem, ind1, ind2, val);
        semop(semid, inc, 1 + (ind1 != ind2) );
    }

    return 0;
}
