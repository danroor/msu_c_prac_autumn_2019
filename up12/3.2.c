#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>

enum
{
    RIGHTS = S_IRUSR | S_IWUSR,
    TXTSZ = 44, //20 digits of -2^64 and "-" sign for each of two numbers,
                //a space and a \n sign
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
    key_t key;
    pid_t *pids, pid;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);

    int msgid = msgget(key, IPC_CREAT | RIGHTS);
    if (msgid < 0) {
        return 1;
    }

    pids = calloc(n, sizeof(*pids));
    if (pids == NULL) {
        return 1;
    }
    
    for (int i = 0; i < n; ++i) {
        pid = fork();
        if (pid < 0) {
            for (int j = 0; j < i; ++j) {
                kill(pids[j], SIGKILL);
            }
 
            free(pids);
            msgctl(msgid, IPC_RMID, NULL);
            return 1;
        }
        if (!pid) {
            num = i + 1;
            break;
        }
        pids[i] = pid;
    }
    free(pids);

    struct message {
        long type;
        char text[TXTSZ];
    } mes;

    memset(&mes, 0, sizeof(mes));    
    
    if (pid) {
        //father
     
        //sending value1 and value2 to process 0
        mes.type = 1;
        snprintf(mes.text, TXTSZ, "%s %s\n", argv[3], argv[4]);
        //printf("%s%d", mes.text, sizeof(mes.text));
        if (msgsnd(msgid, &mes, sizeof(mes.text), NOFLAGS) < 0) {
            //printf("AGAIN %d\n", errno == EAGAIN);
            //printf("ACCES %d\n", errno == EACCES);
            //printf("FAULT %d\n", errno == EFAULT);
            //printf("IDRM %d\n", errno == EIDRM);
            //printf("INTR %d\n", errno == EINTR);
            //printf("INVAL %d\n", errno == EINVAL);
            //printf("NOMEM %d\n", errno == ENOMEM);
            msgctl(msgid, IPC_RMID, NULL);
            return 1;
        }
        
        for (int i = 0; i < n; ++i) {
            wait(NULL);
        }
 
        //message queue already destroyed
        return 0;
    }

    long long x1, x2, maxval;
    sscanf(argv[5], "%lld", &maxval);
 
    while (1) {
        if (msgrcv(msgid, &mes, sizeof(mes.text), num, NOFLAGS) < 0) { 
            if (errno == EIDRM) {
                break;
            }
            return 1;
        }
 
        sscanf(mes.text, "%lld %lld", &x1, &x2);
        x1 += x2; //x3
        printf("%d %lld\n", num - 1, x1);

        if (x1 > maxval) {
            msgctl(msgid, IPC_RMID, NULL);
            break;
        }

        mes.type = x1 % n + 1;
        snprintf(mes.text, TXTSZ, "%lld %lld\n", x2, x1);
        if (msgsnd(msgid, &mes, sizeof(mes.text), NOFLAGS) < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            return 1;
        }
    }

    return 0;
}
