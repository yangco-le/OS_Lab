#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct map {
    unsigned msize;
    char *m_addr;
    struct map *next, *prior;
};

struct maplist {
    struct map *addr;
    int len;
};

struct maplist coremap;
coremap->addr = new map(1000, );

struct map *current_loc = coremap->addr;

char *lmalloc(unsigned size) {
    register char *a;
    register struct map *bp;

    for (bp = current_loc; bp->next != current_loc ; bp = bp->next) {
        if (bp->msize >= size) {
            a = bp->m_addr;
            bp->addr += size;
            bp->msize -= size;
            if (bp->msize == 0) {
                if (coremap->len == 1) {

                } else {
                    bp->prior->next = bp->next;
                    bp->next->prior = bp->prior;
                    struct map *dp = bp;
                    delete dp;
                }
            }
            return a;
        }
    }
}

void lfree(unsigned size, char* addr) {

}

int main() {
    char *mem = malloc(1000);



    return 0;
}
