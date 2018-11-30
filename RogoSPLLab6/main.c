#include <stdio.h>
#include "debug.h"
#include "mem_alloc.h"

int main() {

    struct mem* heap_start = (struct mem*) heap_space_init();

    puts(" empty heap:");
    memalloc_debug_heap(stdout, heap_start);
    char* p1 = rogalloc(4096);
   // for (size_t i = 0; i < 4096; i++) p1[i] = i + 1;
    puts(" p1 = heap_alloc(4096):");
    memalloc_debug_heap(stdout, heap_start);
    rogoFree(p1);
    return 0;
}