#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "ptentry.h"

#define PGSIZE 4096

static int 
err(char *msg, ...) {
    printf(1, "XV6_TEST_OUTPUT %s\n", msg);
    exit();
}

int main(void) {
    const uint PAGES_NUM = 64;
    char *buffer = sbrk(PGSIZE * sizeof(char));
    while ((uint)buffer != 0x6000)
        buffer = sbrk(PGSIZE * sizeof(char));
    
    sbrk(PAGES_NUM * PGSIZE);
    struct pt_entry pt_entries[PAGES_NUM];

    int retval = getpgtable(pt_entries, PAGES_NUM, 0);
    if (retval == PAGES_NUM) {
        for (int i = 0; i < retval; i++) {
            if (dump_rawphymem(pt_entries[i].ppage * PGSIZE, buffer) != 0)
                err("dump_rawphymem return non-zero value\n");
        }
    } else {
        printf(1, "XV6_TEST_OUTPUT: getpgtable returned incorrect value: expected %d, got %d\n", PAGES_NUM, retval);
        exit();
    }
    
    printf(1, "XV6_TEST_OUTPUT PASS!\n");
    exit();
}
