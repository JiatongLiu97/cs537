#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int pid = getpid();
    getnumsyscalls(pid);
    getnumsyscallsgood(pid);
    int checkpoint1 = getnumsyscalls(pid);
    sbrk(4);
    int checkpoint2 = getnumsyscalls(pid);
    printf(1, "XV6_TEST_OUTPUT %d %d\n", checkpoint1, checkpoint2);
    exit();
}
