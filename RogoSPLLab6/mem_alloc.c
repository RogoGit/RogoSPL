//
// Created by dell on 18.11.18.
//
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mem_alloc.h"

char* heap_space_start;

void* heap_space_init() {

    // allocate starting part of memory - one memory page

    heap_space_start = mmap(HEAP_START, MINIMAL_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
    ((struct mem*) heap_space_start)->next = NULL;
    ((struct mem*) heap_space_start)->capacity =  MINIMAL_HEAP_SIZE  - sizeof(struct mem); // heap size without struct attributes
    ((struct mem*) heap_space_start)->is_free = true;
    return (void*) heap_space_start;

}

void rogoFree(void* memChunk) {

    // making one big chunk from several free

    struct mem* chunk = (struct mem*) ((char*) memChunk - sizeof(struct mem));
    chunk->is_free = true;
    while (chunk->next != NULL && chunk->next->is_free) {
        chunk->capacity = chunk->capacity + chunk->next->capacity + sizeof(struct mem);
        chunk->next = chunk->next->next;
    }

}

void* rogalloc(size_t query) {

    if (query % CHUNK_ALIGN != 0) query += CHUNK_ALIGN - (query % CHUNK_ALIGN); //making size to 8

    struct mem* chunk = (struct mem*) heap_space_start;

    //making one big block from lots of empty

    while (true) {

        if (chunk->is_free) {

            while (chunk->next != NULL && chunk->next->is_free) {
                chunk->capacity = chunk->capacity + chunk->next->capacity + sizeof(struct mem);
                chunk->next = chunk->next->next;
            }
                if (chunk->capacity >= query) break;  //stop building block if it is enough
        }

        //making heap bigger

        if (chunk->next == NULL) {

            void* heap_end = (char*) chunk + sizeof(struct mem) + chunk->capacity;

            void* new_area = mmap(heap_end, MINIMAL_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);

            if (new_area != MAP_FAILED && chunk->is_free) {
                chunk->capacity = chunk->capacity + MINIMAL_HEAP_SIZE;
                break;
            }

            else if (new_area == MAP_FAILED) {
                new_area = mmap(NULL, MINIMAL_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
            }

            struct mem* new_chunk_head = (struct mem*) new_area;
            new_chunk_head->next = NULL;
            new_chunk_head->capacity = MINIMAL_HEAP_SIZE - sizeof(struct mem);
            new_chunk_head->is_free = true;
            chunk->next = new_chunk_head;
        }
        else chunk = chunk->next;
    }
    chunk->is_free = false;

    //handling free space after allocation

    size_t remaining_capacity = chunk->capacity - query;

    if (remaining_capacity < CHUNK_MIN_SIZE) {
        //skip header to usable area
        return (void *) (chunk + 1);
    }

    chunk->capacity = query;
    char* chunk_end = ((char*) chunk) + sizeof(struct mem) + query;

    struct mem* new_chunk = (struct mem*) chunk_end;
    new_chunk->next = chunk->next;
    new_chunk->capacity = remaining_capacity - sizeof(struct mem);
    new_chunk->is_free = true;
    chunk->next = (struct mem*) chunk_end;

    return (void*) (chunk + 1);


}