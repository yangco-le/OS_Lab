/******************
****copy right******
******************/

#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define true 1
#define false 0

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

// allocate memory
char *lmalloc(unsigned size, struct maplist coremap, struct map *current_loc);

// free memory
int lfree(unsigned size, char *addr, struct map *current_loc, struct maplist coremap);

// display the result
void display(struct maplist m, char *start_addr);

#endif