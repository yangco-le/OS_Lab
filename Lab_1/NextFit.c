#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// item to record the info of one free memory partition
struct map {
    unsigned m_size;
    char *m_addr;
    struct map *next, *prior;
};

// free memory partition list
struct maplist {
    struct map *addr;
    int len;
};

// initialize a list
static struct maplist coremap;
// record loop position
static struct map *current_loc;

// allocate memory
char *lmalloc(unsigned size) {
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
void lfree(unsigned size, char* addr) {
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

void display(struct maplist m) {
    printf("Current Status:\n");
    struct map *bp = m.addr;
    for (int i = 0;; i++) {
        printf("\tItem %d: [Physical Addr: %u; Logical Addr: %u; Size: %u]\n",
               i, bp->m_addr, bp->m_addr - (char*)m.addr, bp->m_size);
        bp = bp->next;
        if(bp == m.addr)
            break;
    }
}

int main() {
    // memory for storage
    char *mem = malloc(1000);

    // initialization
    coremap.addr = (struct map*)malloc(sizeof(struct map));
    coremap.addr->m_size = 1000;
    coremap.addr->m_addr = (char*) coremap.addr;
    coremap.addr->next = coremap.addr->prior = coremap.addr;

    current_loc = coremap.addr;

    char c;
    char *str;
    while(1) {
        printf("Enter your command: $ ");

        scanf("%s", str);

        if(!strcmp(str, "m") || !strcmp(str, "malloc")) {
            getchar();
            int space1;
            scanf("%d", &space1);
            char *addr1 = lmalloc(space1);
            if(addr1) printf("Memory was successfully allocated to address %u.\n", addr1);
            else printf("The operation failed.\n");
            display(coremap);
        } else if (!strcmp(str, "f") || !strcmp(str, "free")) {
            getchar();
            int space2;
            unsigned int addr2;
            scanf("%d %u", &space2, &addr2);
            lfree(space2, (char*)addr2);
            printf("Memory freed successfully.\n");
            display(coremap);
        } else {
            gets(str);
            printf("Error!\n");
        }

    }
    return 0;
}
