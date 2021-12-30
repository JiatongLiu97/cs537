#ifndef PT_ENTRY_H
#define PT_ENTRY_H
#include "types.h"



/**
 * This structure refers to the state of a virtual page.
 * 
 * A virtual address 'va' has a three-part structure as follows:
 *
 * +--------10------+-------10-------+---------12----------+
 * | Page Directory |   Page Table   | Offset within Page  |
 * |      Index     |      Index     |                     |
 * +----------------+----------------+---------------------+
 *  \--- PDX(va) --/ \--- PTX(va) --/
 * 
 * A page table entry has a structure as follows:
 *
 * +---------------20----------------+---------12----------+
 * |          Physical Page          |     Status Bits     |
 * |             Number              |                     |
 * +----------------+----------------+---------------------+
**/
struct pt_entry {
    /**
     * Page directory index (10 bits) of this virtual page
    */
    uint pdx : 10;

    /**
     * Page Table index (10 bits) of this virtual page
    */
    uint ptx : 10; 

    /**
     * Physical Page Number (20 bits) allocated to this virutal page
    */
    uint ppage : 20; 

    /**
     * This field (1 bit) should be set to 1 if PTE_P == 1, otherwise 0
    */
    uint present : 1;

    /**
     * This field (1 bit) should be set to 1 if PTE_W == 1, otherwise 0
    */
    uint writable : 1;

    /**
     * This field (1 bit) should be set to 1 if this page is currently encrypted, otherwise 0
    */
    uint encrypted : 1;

    uint user : 1;

    uint ref : 1;
};

#endif // __PT_ENTRY_H__
