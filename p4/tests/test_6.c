#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int 
main(void){
    const uint PAGES_NUM = 5;
    // Allocate one pages of space
    char *ptr = sbrk(PGSIZE * sizeof(char));
    ptr = sbrk(PAGES_NUM * PGSIZE);
    int ppid = getpid();

    if (fork() == 0) {
        // Should page fault as normally here
        ptr[PAGES_NUM * PGSIZE] = 0xAA;
        printf(1, "XV6_TEST_OUTPUT Seg fault failed to trigger\n");
        // Shouldn't reach here
        kill(ppid);
        exit();
    } else {
        wait();
    }

    printf(1, "XV6_TEST_OUTPUT TEST PASS\n");

    exit();
}
