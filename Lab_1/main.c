#include "NextFit.h"

// initialize a list
static struct maplist coremap;
// record loop position
static struct map *current_loc;

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
            char *addr1 = lmalloc(space1, coremap, current_loc);
            if(addr1) printf("Memory was successfully allocated to address %lu.\n", (unsigned long)(addr1));
            else printf("The operation failed.\n");
            display(coremap);
        } else if (!strcmp(str, "f") || !strcmp(str, "free")) {
            getchar();
            int space2;
            unsigned long addr2;
            scanf("%d %lu", &space2, &addr2);
            lfree(space2, (char*)addr2, current_loc);
            printf("Memory freed successfully.\n");
            display(coremap);
        } else {
            fgets(str,100,stdin);  // set a limit in case of unlimited input
            printf("Error!\n");
        }

    }
    return 0;
}
