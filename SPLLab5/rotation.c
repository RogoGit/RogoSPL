#include <stdlib.h>
#include <stdio.h>
#include "rotation.h"

struct image* rotate(struct image const* rotatable){

    int row;
    int col;

    struct image* rotated = (struct image*)malloc(sizeof(struct image));
    rotated->width = rotatable->height;
    rotated->height = rotatable->width;
    rotated->data = (struct pixel*)malloc(rotatable->height*rotatable->width* sizeof(struct pixel));

    for (row = 0; row < rotatable->height; row++){
        for (col = 0; col < rotatable->width; col++) {
            rotated->data[((rotatable->width - 1 - col) * rotatable->height) + row] = rotatable->data[row*rotatable->width + col];
        }
    }
    return rotated;
}

int compute_total_padding_rotated(struct bmp_header* header) {
    return header->biWidth * (header->biHeight % 4);
}

void rotate_right_padded(const uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height) {
    for (uint32_t h = 0; h < height; h++) {
        uint64_t src_padding = h * (width % 4);

        for (uint32_t w = 0; w < width; w++) {
            uint64_t dst_padding = (width - w) * (height % 4);

            *(struct pixel*)(dst + (((width - 1 - w) * height) + h) * sizeof(struct pixel) + dst_padding) =
                    *(struct pixel*)(src + (h * width + w) * sizeof(struct pixel) + src_padding);
        }
    }
}