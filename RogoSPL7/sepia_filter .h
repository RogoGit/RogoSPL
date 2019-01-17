//
// Created by dell on 16.12.18.
//

#ifndef ROGOSPL7_sepia_filter_sse_H
#define ROGOSPL7_sepia_filter_sse_H

#include "BMP_Struct.h"
#include "BMP_IO.h"
#include <stdint.h>

void sepia_c_inplace( struct image* img );

void sepia_sse_inplace(struct image* img);

#endif //ROGOSPL7_sepia_filter_sse_H
