#include "NextFit.h"

// allocate memory
char *lmalloc(unsigned size, struct maplist coremap, struct map *current_loc) {
    char *a;
    struct map *bp = current_loc;

    while(1) {
        if (bp->m_size >= size) {
            a = bp->m_addr;
            bp->m_addr += size;
            bp->m_size -= size;

            if (bp->m_size == 0) {
                // the partition is full
                if (coremap.len == 1) {
                    // only one item in the list
                    coremap.addr = NULL;
                    struct map *dp = bp;
                    free(dp);
                } else {
                    bp->prior->next = bp->next;
                    bp->next->prior = bp->prior;
                    struct map *dp = bp;
                    free(dp);
                }
                coremap.len--;
            }

            return a;
        }
        bp = bp->next;
        if(bp == current_loc)
            break;
    }

    return NULL;
}

// free memory
void lfree(unsigned size, char* addr, struct map *current_loc) {
    struct map *bp = current_loc;

    // find the next free part
    while (bp->m_addr <= addr) {
        bp = bp->next;
        if(bp == current_loc)
            break;
    }

    if (bp->prior->m_addr + bp->prior->m_size == addr && addr + size != bp->m_addr) {
        // case 1
        bp->prior->m_size += size;
    } else if (bp->prior->m_addr + bp->prior->m_size == addr && addr + size == bp->m_addr) {
        // case 2
        bp->prior->m_size += size + bp->m_size;
        bp->prior->next = bp->next;
        bp->next->prior = bp->prior;
        struct map *dp = bp;
        free(dp);
    } else if (bp->prior->m_addr + bp->prior->m_size != addr && addr + size == bp->m_addr) {
        // case 3
        bp->m_size += size;
        bp->m_addr -= size;
    } else if (bp->prior->m_addr + bp->prior->m_size != addr && addr + size != bp->m_addr) {
        // case 4
        struct map *p = (struct map*)malloc(sizeof(struct map));
        p->m_size = size;
        p->m_addr = addr;
        p->next = bp;
        p->prior = bp->prior;
        bp->prior->next = p;
        bp->prior = p;
    }
}

// display the result
void display(struct maplist m) {
    printf("Current Status:\n");
    struct map *bp = m.addr;
    for (int i = 0;; i++) {
        printf("\tItem %d: [Physical Addr: %lu; Logical Addr: %lu; Size: %u]\n",
               i, bp->m_addr, bp->m_addr - (char*)m.addr, bp->m_size);
        bp = bp->next;
        if(bp == m.addr)
            break;
    }
}
