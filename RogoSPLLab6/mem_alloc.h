//
// Created by dell on 18.11.18.
//
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef ROGOSPLLAB6_MEM_ALLOC_H
#define ROGOSPLLAB6_MEM_ALLOC_H

struct mem {
    struct mem* next;
    size_t capacity;
    bool is_free;
};

#endif //ROGOSPLLAB6_MEM_ALLOC_H
