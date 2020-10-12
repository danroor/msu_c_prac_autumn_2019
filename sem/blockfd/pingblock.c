#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <poll.h>
#include <stdio.h>
#include <signal.h>

void work1(int fdr, int fdw)
{
    int x = 1; 
    write(fdw, &x, sizeof(x));
    while (1) {
        //printf("test\n"); fflush(stdout);
        int x;
        read(fdr, &x, sizeof(x));
        printf("%d %d\n", getpid(), x++);
        fflush(stdout);
        write(fdw, &x, sizeof(x));
    }
}

void work2(int fdr, int fdw, int sfd)
{
    int x = 1;
    write(fdw, &x, sizeof(x));
    kill(getppid(), SIGUSR1);
    while (1) {
        //printf("test\n"); fflush(stdout);
        int x;
        read(fdr, &x, sizeof(x));
        printf("%d %d\n", getpid(), x++);
        fflush(stdout);
        write(fdw, &x, sizeof(x));
    }
}

int main(void)
{
    int p01[2], p10[2], p02[2];

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    pipe(p01);
    pipe(p10);
  
    if (!fork()) {
        close(p01[1]); close(p10[0]);
        work1(p01[0], p10[1]);
        _exit(1);
    }

    close(p01[0]); close(p10[1]);

    int sfd = sigalfd(-1, &mask, 0);
    pipe(p02);

    if (!fork()) {
        close(p01[1]); close(p10[0]);
        work2(p02[0], p02[1], sfd);
        _exit(1);
    }

    int efd = epoll_create1(0);
    
    epoll_ctl(efd, EPOLL_CTL_ADD, p10[0], &(struct epoll_event) 
            { .events = EPOLLIN, .data = { .fd = p10[0] }  } );
    while (1) {
        //printf("test\n"); fflush(stdout);
        enum { EVENT_SIZE = 10 };
        struct epoll_event evs[EVENT_SIZE];
        int n = epoll_wait(efd, evs, EVENT_SIZE, -1);
   
        for (int  i = 0; i < n; ++i) {
            if (evs[i].data.fd == p10[0]) { //p10[0] ready for reading
                int x;
                read(p10[0], &x, sizeof(x));
                printf("PARENT %d\n", x); fflush(stdout);
                ++x;
                write(p01[1], &x, sizeof(x));
            }
        }
    }
}
