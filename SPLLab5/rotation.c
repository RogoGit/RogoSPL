//
// Created by dell on 02.11.18.
//

#include <stdint.h>
#include <stdlib.h>
#include "rotation.h"
#include "bmp_pic_struct.h"

struct image* rotate(const struct image* pic) {

    uint32_t row;
    uint32_t col;

    struct image* rotated = (struct image*)malloc(sizeof(struct image));
    rotated->data = (struct pixel*)malloc( pic->height * pic->width* sizeof(struct pixel));
    rotated->width = pic->height;
    rotated->height = pic->width;

    for (row = 0;  row < pic->height; row++){
        for (col = 0; col < pic->width; col++){
            rotated->data[(pic->width - 1 - col)*pic->height + row] = pic->data[row*pic->width + col];
        }
    }
    return rotated;

}