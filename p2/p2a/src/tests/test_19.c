#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int p0[2], p1[2];
    pipe(p0);
    pipe(p1);
    int rnt = fork();
    if (rnt == 0) {
        // Child Process
        read(-1, 0, 0);
        read(-1, 0, 0);
        read(-1, 0, 0);
        write(p0[1], "checkpoint1", 11);
        char buffer[128];

        // Child Process block here until parent process hit checkpoint2(after printing out the message).
        int bytes = read(p1[0], buffer, 128);
        printf(1, "XV6_TEST_OUTPUT [child] %d %s\n", bytes, buffer);
        exit();
    } else {
        // Parent Process
        char buffer[128];
        // Parent Process block here unitl child process hit checkpoint1.
        int bytes = read(p0[0], buffer, 128);
        printf(1, "XV6_TEST_OUTPUT [parent] %d %s\n", bytes, buffer);
        printf(1, "XV6_TEST_OUTPUT [parent] child getnumsyscalls %d\n", getnumsyscalls(rnt));
        printf(1, "XV6_TEST_OUTPUT [parent] child getnumsyscallsgood %d\n", getnumsyscallsgood(rnt));
        write(p1[1], "checkpoint2", 11);
    }
    wait();
    // Child process pid become unused now after wait();
    printf(1, "XV6_TEST_OUTPUT [parent] child getnumsyscalls %d\n", getnumsyscalls(rnt));
    printf(1, "XV6_TEST_OUTPUT [parent] child getnumsyscallsgood %d\n", getnumsyscallsgood(rnt));

    exit();
}
