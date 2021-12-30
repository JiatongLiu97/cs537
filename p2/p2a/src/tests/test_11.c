#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int pid = getpid();
    char *dir_name = "/bin";
    int checkpoint0 = getnumsyscallsgood(pid);
    mkdir(dir_name);
    int checkpoint1 = getnumsyscallsgood(pid);
    chdir(dir_name);
    int checkpoint2 = getnumsyscallsgood(pid);
    printf(1, "XV6_TEST_OUTPUT %d %d %d\n", checkpoint0, checkpoint1, checkpoint2);
    exit();
}
