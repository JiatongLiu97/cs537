#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int pid = getpid();
    char *dir_name = "test_dir";
    int checkpoint0 = getnumsyscalls(pid);
    mkdir(dir_name);
    int checkpoint1 = getnumsyscalls(pid);
    chdir(dir_name);
    int checkpoint2 = getnumsyscalls(pid);
    printf(1, "XV6_TEST_OUTPUT %d %d %d\n", checkpoint0, checkpoint1, checkpoint2);
    exit();
}
