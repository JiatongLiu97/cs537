#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(void)
{

    for (int i = 0; i < 200; ++i)
        getpid();
    int fd = open("/bin/wc", O_RDONLY);
    char buffer[100];
    read(fd, buffer, 100);
    
    for (int i = 0; i < 200; ++i)
        read(-1, 0, 0);

    for (int i = 0; i < 200; ++i)
        read(fd, buffer, -1);
    int pid = getpid();
    printf(1, "XV6_TEST_OUTPUT %d %d\n", getnumsyscalls(pid), getnumsyscallsgood(pid));
    exit();
}
