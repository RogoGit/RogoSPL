//
// Created by dell on 18.11.18.
//
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

#ifndef ROGOSPLLAB6_MEM_ALLOC_H
#define ROGOSPLLAB6_MEM_ALLOC_H

#define HEAP_START ((void*)0x04040000)
#define MINIMAL_BLOCK_SIZE ((char) 2)
#define CHUNK_ALIGN 8
#define CHUNK_MIN_SIZE (sizeof(chunk_head_t) + CHUNK_ALIGN)

#define _USE_MISC

struct mem {
    struct mem* next;
    size_t capacity;
    bool is_free;
};

/*#pragma pack(push, 1)
struct mem {
    struct mem* next;
    size_t capacity;
    bool is_free;
};
#pragma pack(pop)*/

void* rogalloc( size_t query );
void rogoFree( void* memChunk );
void* heap_init( size_t initial_size );
#define DEBUG_FIRST_BYTES 4
void memalloc_debug_struct_info( FILE* f,
                                 struct mem const* const address );
void memalloc_debug_heap( FILE* f, struct mem const* ptr );


#endif //ROGOSPLLAB6_MEM_ALLOC_H
