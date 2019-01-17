#include <stdio.h>
#include "debug.h"
#include "mem_alloc.h"

int main() {

    struct mem* heap_start = (struct mem*) heap_space_init();

    puts(" empty heap:");
    memalloc_debug_heap(stdout, heap_start);


  /*  char* p0 = rogalloc(-8);
  //  if (p0!= NULL) {
    printf(" p0 = %p = rogalloc(-8):\n", p0);
    memalloc_debug_heap(stdout, heap_start);// }
   // else {puts("Allocation error");} */

    char* p2 = rogalloc(33);
    printf(" p2 = %p = rogalloc(33):\n", p2);
    memalloc_debug_heap(stdout, heap_start);

 //   puts(" free(p1):");
   // rogoFree(p1);

    char* p3 = rogalloc(2);
    printf(" p3 = %p = rogalloc(2):\n", p3);
    memalloc_debug_heap(stdout, heap_start);

    char* p0 = rogalloc(-8);
    printf(" p0 = %p = rogalloc(-8):\n", p0);
    memalloc_debug_heap(stdout, heap_start);

    return 0;
}

//p0 = 0x4040018 = rogalloc(-8):
//start: 0x4040000, size: 18446744073709551608, is_free: 0
