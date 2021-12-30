#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    printf(1, "XV6_TEST_OUTPUT %d\n", getnumsyscallsgood(getpid()));
    exit();
}
