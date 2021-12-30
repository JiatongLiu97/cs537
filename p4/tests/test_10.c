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


void access_all_dummy_pages(char **dummy_pages, uint len) {
    for (int i = 0; i < len; i++) {
        dummy_pages[i][0] = dummy_pages[i][0];
    }
    printf(1, "\n");
}

int main(void) {
    const uint PAGES_NUM = 32;
    const uint expected_dummy_pages_num = 4;
    // These pages are used to make sure the test result is consistent for different text pages number
    char *dummy_pages[expected_dummy_pages_num];
    char *buffer = sbrk(PGSIZE * sizeof(char));
    char *sp = buffer - PGSIZE;
    char *boundary = buffer - 2 * PGSIZE;
    struct pt_entry pt_entries[PAGES_NUM];

    uint text_pages = (uint) boundary / PGSIZE;
    if (text_pages > expected_dummy_pages_num - 1)
        err("XV6_TEST_OUTPUT: program size exceeds the maximum allowed size. Please let us know if this case happens\n");
    
    for (int i = 0; i < text_pages; i++)
        dummy_pages[i] = (char *)(i * PGSIZE);
    dummy_pages[text_pages] = sp;

    for (int i = text_pages + 1; i < expected_dummy_pages_num; i++)
        dummy_pages[i] = sbrk(PGSIZE * sizeof(char));
    

    // After this call, all the dummy pages including text pages and stack pages
    // should be resident in the clock queue.
    access_all_dummy_pages(dummy_pages, expected_dummy_pages_num);

    // Bring the buffer page into the clock queue
    buffer[0] = buffer[0];

    // Now we should have expected_dummy_pages_num + 1 (buffer) pages in the clock queue
    // Fill up the remainig slot with heap-allocated page
    // and bring all of them into the clock queue
    int heap_pages_num = CLOCKSIZE - expected_dummy_pages_num - 1;
    char *ptr = sbrk(heap_pages_num * PGSIZE * sizeof(char));
    for (int i = 0; i < heap_pages_num; i++) {
      for (int j = 0; j < PGSIZE; j++) {
        ptr[i * PGSIZE + j] = 0x00;
      }
    }
    
    // An extra page which will trigger the page eviction
    char* extra_pages = sbrk(PGSIZE * sizeof(char));
    for (int j = 0; j < PGSIZE; j++) {
      extra_pages[j] = 0x00;
    }

    // Deallocate the extra page
    sbrk(-1 * PGSIZE);

    // Bring all the dummy pages and buffer back to the 
    // clock queue and reset their ref to 1
    // No eviction should happen this time
    access_all_dummy_pages(dummy_pages, expected_dummy_pages_num);
    buffer[0] = buffer[0];
    sbrk(PGSIZE);

    int retval = getpgtable(pt_entries, heap_pages_num, 1);
    if (retval == heap_pages_num) {
      for (int i = 0; i < retval; i++) {
          printf(1, "XV6_TEST_OUTPUT Index %d: pdx: 0x%x, ptx: 0x%x, writable bit: %d, encrypted: %d, ref: %d\n", 
              i,
              pt_entries[i].pdx,
              pt_entries[i].ptx,
              pt_entries[i].writable,
              pt_entries[i].encrypted,
              pt_entries[i].ref
          ); 
          
          uint expected = 0x00;
          if (pt_entries[i].encrypted)
            expected = ~0x00;

          if (dump_rawphymem(pt_entries[i].ppage * PGSIZE, buffer) != 0)
              err("dump_rawphymem return non-zero value\n");
          
          for (int j = 0; j < PGSIZE; j++) {
              if (buffer[j] != (char)expected) {
                    printf(1, "XV6_TEST_OUTPUT: content is incorrect at address 0x%x: expected 0x%x, got 0x%x\n", ((uint)(pt_entries[i].pdx) << 22 | (pt_entries[i].ptx) << 12) + j ,expected & 0xFF, buffer[j] & 0xFF);
                    exit();
              }
          }

      }
    } else
        printf(1, "XV6_TEST_OUTPUT: getpgtable returned incorrect value: expected %d, got %d\n", heap_pages_num, retval);
    
    exit();
}