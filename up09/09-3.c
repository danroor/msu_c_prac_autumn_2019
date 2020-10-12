#include <unistd.h>
#include <sys/wait.h>

//1 - successfully finished
//0 - otherwise
int
launch(char *exe)
{
    if (!fork()) {
        execlp(exe, exe, NULL);
        _exit(1);
        //the son process (not the launch function)
        //returns 1
    }
    
    int status;
    wait(&status);
    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int
main(int argc, char *argv[])
{
    if (launch(argv[1])) {
        return !launch(argv[3]);
    }
    if (launch(argv[2])) {
        return !launch(argv[3]);
    }
    
    return 1;
}
