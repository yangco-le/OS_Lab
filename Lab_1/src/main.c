#include "NextFit.h"

// initialize a list
static struct maplist coremap;
// record loop position
static struct map *current_loc;

int main() {

    // initialization
    coremap.addr = (struct map*)malloc(sizeof(struct map));
    coremap.addr->m_size = 1000;
    coremap.addr->m_addr = (char*) malloc(1000); // malloc 1000 bytes storage
    coremap.addr->next = coremap.addr->prior = coremap.addr;

    current_loc = coremap.addr;
    char *start_addr = coremap.addr->m_addr; // record start address

    char str[10];
    while(scanf("%s", str) != EOF) {
        if(!strcmp(str, "m") || !strcmp(str, "malloc")) {
            getchar();
            int space1;
            scanf("%d", &space1);
            char *addr1 = lmalloc(space1, &coremap, &current_loc);
            if(addr1) printf("Memory was successfully allocated to address %lu.\n", (unsigned long)(addr1));
            else printf("The operation failed.\n");
            display(coremap, start_addr);
        } else if (!strcmp(str, "f") || !strcmp(str, "free")) {
            getchar();
            int space2;
            int addr2; // logical address
            scanf("%d %d", &space2, &addr2);
            if(lfree(space2, start_addr + addr2, &coremap))
                printf("Memory freed successfully.\n");
            else printf("Error!\n");
            display(coremap, start_addr);
        } else {
            fgets(str,100,stdin);  // set a limit in case of unlimited input
            printf("Error!\n");
        }

    }
    return 0;
}
