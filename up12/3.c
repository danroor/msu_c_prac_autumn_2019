#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

enum
{
    RIGHTS = S_IRUSR | S_IWUSR,
    TXTSZ = 44, //20 digits of -2^64 and "-" sign for each of two numbers,
                //a space and a \0 sign
    NOFLAGS = 0,
    ARGS = 6
};

int
main(int argc, char *argv[])
{
    if (argc < ARGS) {
        return 1;
    }

    int n, num;
    long long value1, value2, maxval;
    key_t key;
    pid_t *pids, pid;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[5], "%lld", &maxval);

    pids = calloc(n, sizeof(*pids));
    if (pids == NULL) {
        return 1;
    }
    int msgid = msgget(key, IPC_CREAT | RIGHTS);
    
    for (int i = 0; i < n; ++i) {
        pid = fork();
        if (pid < 0) {
            msgctl(msgid, IPC_RMID, NULL);

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
  
    struct message {
        long type;
        char text[TXTSZ];
    } mes;
 
    memset(&mes, 0, sizeof(mes));    
    
    if (pid) {
        //father

        //mes.type == 0 (memset)        
        //sending value1 and value2 to process 0
        snprintf(mes.text, TXTSZ, "%s %s", argv[3], argv[4]);
        msgsnd(msgid, &mes, MSGMAX, NOFLAGS);
        
        int status;
        for (int i = 0; i < n; ++i) {
            wait(&status);
            if (!WIFEXITED(status) || !WEXITSTATUS(status)) {
                
            }
        }
 
        //message queue already destroyed
        return 0;
    }

    //sons

    while (1) {
        if (msgrcv() < 0) {
            return 1;
        }
    }

    return 0;
}
