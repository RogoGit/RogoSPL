#include <stdio.h>
#include "debug.h"
#include "mem_alloc.h"

int main() {

    struct mem* heap_start = (struct mem*) heap_space_init();

    printf("%lu", sizeof(struct mem));

    puts(" empty heap:");
    memalloc_debug_heap(stdout, heap_start);

    char* p1 = rogalloc(4096);
    printf(" p1 = %p = rogalloc(4096):\n", p1);
    memalloc_debug_heap(stdout, heap_start);

    char* p2 = rogalloc(33);
    printf(" p2 = %p = rogalloc(33):\n", p2);
    memalloc_debug_heap(stdout, heap_start);

    puts(" free(p1):");
    rogoFree(p1);

    char* p3 = rogalloc(2);
    printf(" p3 = %p = rogalloc(2):\n", p3);
    memalloc_debug_heap(stdout, heap_start);

    return 0;
}