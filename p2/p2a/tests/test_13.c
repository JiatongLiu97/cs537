#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int pid = getpid();
    int checkpoint0 = getnumsyscallsgood(pid);
    link("/bin/no", "/bin/no");
    int checkpoint1 = getnumsyscallsgood(pid);
    unlink("/bin/no");
    int checkpoint2 = getnumsyscallsgood(pid);
    printf(1, "XV6_TEST_OUTPUT %d %d %d\n", checkpoint0, checkpoint1, checkpoint2);
    exit();
}
