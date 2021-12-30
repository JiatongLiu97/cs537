#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int p[2];
    char* argv[2];
    argv[0] = "wc";
    argv[1] = 0;

    pipe(p);
    if (fork() == 0) {
        int pid = getpid();
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        read(p[0],0,0);
        read(p[1],0,0);
        exec("/bin/wc", argv);
        printf(1, "XV6_TEST_OUTPUT child %d\n", getnumsyscallsgood(pid));
        exit();
    } else  {
        write(p[1], "hello world\n", 12);
        close(p[0]);
        close(p[1]);
        write(p[1], "hello world\n", 12);
    }
    wait();
    printf(1, "XV6_TEST_OUTPUT parent %d\n", getnumsyscallsgood(getpid()));
    exit();
}
