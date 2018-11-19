//
// Created by dell on 19.11.18.
//

#include <stdio.h>
#include "mem_alloc.h"

#ifndef ROGOSPLLAB6_DEBUG_H
#define ROGOSPLLAB6_DEBUG_H

void memalloc_debug_struct_info( FILE* f, struct mem const* const address );

void memalloc_debug_heap( FILE* f, struct mem const* ptr );

#endif //ROGOSPLLAB6_DEBUG_H
