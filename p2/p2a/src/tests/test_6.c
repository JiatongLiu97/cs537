#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
    for (int i = 1; i < NPROC; i ++){ 
        printf(1, "XV6_TEST_OUTPUT PID:%d %d\n", i, getnumsyscalls(i));
    }
    exit();
}
