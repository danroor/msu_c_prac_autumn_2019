#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <fcntl.h>

enum
{
    RIGHTS = S_IRUSR | S_IWUSR,
    ARGS = 2
};

int
main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    if (argc < ARGS) {
        return 1;
    }
    
    int count, num;
    sscanf(argv[1], "%d", &count);
    
    key_t key = ftok(argv[0], 's');
    if (key == -1) {
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

    int semid = semget(key, count, IPC_CREAT | RIGHTS);
    semctl(semid, count, SETALL, set);
    free(set.array);

    struct sembuf inc, dec;
    memset(&inc, 0, sizeof(inc));
    memset(&dec, 0, sizeof(dec));
    dec.sem_op = -1;
    inc.sem_op = 1;
    
    pid_t pid, *pids;
    pids = calloc(count, sizeof(*pids));
    if (pids == NULL) {
        return 1;
    }
    
    for (int i = 0; i < count; ++i) {
        pid = fork();
        if (pid < 0) {
            for (int j = 0; j < i; ++j) {
                kill(pids[j], SIGKILL);
                wait(NULL);
            }

            semctl(semid, 0, IPC_RMID, set);
            return 1;
        }
        if (!pid) {         
            free(pids);
            num = i;
            break;
        }
        pids[i] = pid;
    }    

    if (pid) {
        //father
        semop(semid, &inc, 1); //unblock proc #0
        
        pid = wait(NULL);
        for (int i = 0; i < count - 1; ++i) {
            if (pid != pids[i]) {
                kill(pids[i], SIGKILL);
                wait(NULL);
            }
        }
        free(pids);
        semctl(semid, 0, IPC_RMID, set);
        return 0;
    }

    int x;
    dec.sem_num = num;
    while (1) {
        semop(semid, &dec, 1);
        if (scanf("%d", &x) != 1) {
            break;
        }

        printf("%d %d\n", num, x);
        fflush(stdout);
        inc.sem_num = (x % count + count) % count;
        semop(semid, &inc, 1);
    }

    return 0;
}
