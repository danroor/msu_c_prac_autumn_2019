#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>

enum
{
    RIGHTS = S_IRUSR | S_IWUSR,
    NOFLAGS = 0,
    ARGS = 6,
    TXTSZ = 44 //20 digits, "-" sign for each number, a space and a \n sign
};

int
main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    if (argc < ARGS) {
        return 1;
    }

    int n, num;
    key_t key;
    pid_t pid;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);
 
    long long x1, x2, maxval;
    sscanf(argv[5], "%lld", &maxval);

    int msgid = msgget(key, IPC_CREAT | RIGHTS);
    if (msgid < 0) {
        return 1;
    }
    
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
            num = i + 1;
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
        //sending value1 and value2 to process 0
        mes.type = 1;
        snprintf(mes.text, TXTSZ, "%s %s\n", argv[3], argv[4]);
        if (msgsnd(msgid, &mes, sizeof(mes.text), NOFLAGS) < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            for (int j = 0; j < n; ++j) {
                wait(NULL);
            }
            return 1;
        }
        
        int status, ret = 0;
        for (int i = 0; i < n; ++i) {
            wait(&status);
            ret |= !WIFEXITED(status) || WEXITSTATUS(status);
        }
 
        //message queue already removed by a son process
        return ret;
    }
 
    while (1) {
        if (msgrcv(msgid, &mes, sizeof(mes.text), num, NOFLAGS) < 0) { 
            if (errno == EIDRM) {
                errno = 0;
                break;
            }
            errno = 0;
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
