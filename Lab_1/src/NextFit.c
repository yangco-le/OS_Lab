#include "NextFit.h"

// allocate memory
char *lmalloc(unsigned size, struct maplist *coremap, struct map* *current_loc) {
    char *a;
    struct map *bp = *current_loc;

    while(1) {
        if (bp->m_size >= size) {
            a = bp->m_addr;
            bp->m_addr += size;
            bp->m_size -= size;

            if (bp->m_size == 0) {
                // the partition is full
                if (coremap->len == 1) {
                    // only one item in the list
                    coremap->addr = NULL;
                    struct map *dp = bp;
                    free(dp);
                } else {
                    bp->prior->next = bp->next;
                    bp->next->prior = bp->prior;
                    struct map *dp = bp;
                    free(dp);
                }
                coremap->len--;
            }
            *current_loc = bp;
            return a;
        }
        bp = bp->next;
        if(bp == *current_loc)
            break;
    }

    return NULL;
}

// free memory
int lfree(unsigned size, char* addr, struct maplist *coremap) {
    if(size <= 0) return false;

    struct map *bp = coremap->addr;

    // find the next free part
    while (bp->m_addr <= addr) {
        bp = bp->next;
        if(bp == coremap->addr)
            break;
    }

    if (bp->prior->m_addr + bp->prior->m_size == addr && addr + size != bp->m_addr) {
        // case 1
        // printf("1\n");
        bp->prior->m_size += size;
        return true;
    } else if (bp->prior->m_addr + bp->prior->m_size == addr && addr + size == bp->m_addr) {
        // case 2
        // printf("2\n");
        bp->prior->m_size += size + bp->m_size;
        bp->prior->next = bp->next;
        bp->next->prior = bp->prior;
        if(coremap->addr == bp) coremap->addr =  bp->prior;
        coremap->len--;
        struct map *dp = bp;
        free(dp);
        return true;
    } else if (bp->prior->m_addr + bp->prior->m_size != addr && addr + size == bp->m_addr) {
        // case 3
        // printf("3\n");
        bp->m_size += size;
        bp->m_addr -= size;
        return true;
    } else if (bp->prior->m_addr + bp->prior->m_size != addr && addr + size != bp->m_addr) {
        // case 4
        // printf("4\n");
        struct map *p = (struct map*)malloc(sizeof(struct map));
        p->m_size = size;
        p->m_addr = addr;
        p->next = bp;
        p->prior = bp->prior;
        bp->prior->next = p;
        bp->prior = p;
        coremap->len++;
        if(addr < coremap->addr->m_addr) coremap->addr = p;
        return true;
    }
    return false;
}

// display the result
void display(struct maplist m, char *start_addr) {
    printf("--------------------------------------------------------------------------------\n");
    printf("\t\tCurrent Status(Start Address: %lu)\n", (unsigned long)start_addr);
    printf("--------------------------------------------------------------------------------\n");
    struct map *bp = m.addr;
    for (int i = 0;; i++) {
            printf("    Item %d: [Physical Addr: %lu; Logical Addr: %lu; Size: %u]\n",
               i, (unsigned long)bp->m_addr, (unsigned long)(bp->m_addr - start_addr), bp->m_size);
        bp = bp->next;
        if(bp == m.addr)
            break;
    }
    printf("--------------------------------------------------------------------------------\n\n");
}
