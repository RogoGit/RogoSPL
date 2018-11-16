#include <stdlib.h>
#include <stdio.h>
#include "rotation.h"

struct image* rotate(struct image const* image){
    struct image* new_image = (struct image*)malloc(sizeof(struct image));
    new_image->data = (struct pixel*)malloc(image->height*image->width* sizeof(struct pixel));
    new_image->width = image->height;
    new_image->height = image->width;

    uint32_t i, j;
    for (i = 0; i < image->height; ++i){
        for (j = 0; j < image->width; ++j) {
            *(new_image->data + ((image->width - 1 - j) * image->height) + i) = *(image->data + i*image->width + j);
        }
    }
    return new_image;
}
