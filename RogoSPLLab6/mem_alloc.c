//
// Created by dell on 18.11.18.
//
#include <stdlib.h>
#include <stdbool.h>
#include "mem_alloc.h"

char* heap_space_start;

void* heap_space_init() {
    heap_space_start = mmap(HEAP_START, MINIMAL_BLOCK_SIZE + sizeof(struct mem), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
    ((struct mem*) heap_space_start)->next = NULL;
    ((struct mem*) heap_space_start)->capacity =  MINIMAL_BLOCK_SIZE;       //Takoi li?????
    ((struct mem*) heap_space_start)->is_free = true;
    return (void*) heap_space_start;
}

void rogoFree(void* memChunk) {

    struct mem* chunk = (struct mem*) ((char*) memChunk - sizeof(struct mem));
    chunk->is_free = true;
    while (chunk->next != NULL && chunk->next->is_free) {
        chunk->capacity = chunk->capacity + chunk->next->capacity + sizeof(struct mem);
        chunk->next = chunk->next->next;
    }

}

void* rogalloc(size_t query) {

    if (query % CHUNK_ALIGN != 0) { query = query + CHUNK_ALIGN - (query % CHUNK_ALIGN); }
    struct mem* chunk = (struct mem*) heap_space_start;
    while (true) {
        if (chunk->is_free) {
            if (chunk->capacity >= query) break;
                chunk->capacity = chunk->capacity + chunk->next->capacity + sizeof(struct mem);
                chunk->next = chunk->next->next;
            if (chunk->capacity >= query) break;
        }
        //if (chunk->next == NULL) reserve_new_chunk(chnk);
        else chunk = chunk->next;
    }
    chunk->is_free = false;

 /*   size_t remaining_capacity = chnk->capacity - requested_size;
     ---If we cannot fit a new chunk into the free space at the end of the chunk,
     * there is nothing we can do but leave the capacity as is.
    if (remaining_capacity < CHUNK_MIN_SIZE)
        --- chnk + 1 skips the chunk header — we return a pointer to the usable area ---
        return (void*) (chnk + 1);

    chnk->capacity = requested_size;
    char* chnk_end = ((char*) chnk) + sizeof(chunk_head_t) + requested_size;
    chunk_head_t* succ_chnk = (chunk_head_t*) chnk_end;
    succ_chnk->next = chnk->next;
    succ_chnk->capacity = remaining_capacity - sizeof(chunk_head_t);
    succ_chnk->is_free = true;
    chnk->next = (chunk_head_t*) chnk_end;
    return (void*) (chnk + 1); */

}