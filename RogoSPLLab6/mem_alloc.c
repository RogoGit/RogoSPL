//
// Created by dell on 18.11.18.
//
#include <stdlib.h>
#include <stdbool.h>
#include "mem_alloc.h"

void rogoFree(void* memChunk) {

    struct mem* chunk = (struct mem*) ((char*) memChunk - sizeof(struct mem));
    chunk->is_free = true;
    while (chunk->next != NULL && chunk->next->is_free) {
        chunk->capacity = chunk->capacity + chunk->next->capacity + sizeof(struct mem);
        chunk->next = chunk->next->next;
    }

}

void* rogalloc(size_t query) {

}