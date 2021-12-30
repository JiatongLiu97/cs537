#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    int pid = getpid();
    int checkpoint0 = getnumsyscalls(pid);
    link("/bin/wc_new", "/bin/wc");
    int checkpoint1 = getnumsyscalls(pid);
    unlink("/bin/wc_new");
    int checkpoint2 = getnumsyscalls(pid);
    printf(1, "XV6_TEST_OUTPUT %d %d %d\n", checkpoint0, checkpoint1, checkpoint2);
    exit();
}
