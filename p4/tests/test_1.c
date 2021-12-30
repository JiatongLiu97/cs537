#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "ptentry.h"

#define PGSIZE 4096

int main(void) {
    const uint PAGES_NUM = 100;
    // Allocate one pages of space
    char *buffer = sbrk(PGSIZE * sizeof(char));
    char *sp = buffer - PGSIZE;
    char *boundary = buffer - 2 * PGSIZE;
    char *text = 0x0;
    uint text_pages = (uint) boundary / PGSIZE;
    struct pt_entry pt_entries[PAGES_NUM];
    sbrk(PAGES_NUM * PGSIZE);

    for (int i = 0; i < text_pages; i++)
        text[i * PGSIZE] = text[i * PGSIZE];
    sp[0] = sp[0];
    buffer[0] = buffer[0];
    int expected_pages_num = (uint)buffer / PGSIZE;


    int retval = getpgtable(pt_entries, 100, 1);
    if (retval != expected_pages_num) {
        printf(1, "XV6_TEST_OUTPUT: getpgtable returned incorrect value: expected %d, got %d\n", expected_pages_num, retval);
        exit();
    }
    printf(1, "XV6_TEST_OUTPUT PASS!\n");
    exit();
}
