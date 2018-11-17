//
// Created by dell on 02.11.18.
//

#ifndef SPLLAB5_ROTATION_H
#define SPLLAB5_ROTATION_H
#include "bmp_pic_struct.h"

struct image* rotate(struct image const* image);

int compute_total_padding_rotated(struct bmp_header* header);

void rotate_right_padded(const uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);

#endif //SPLLAB5_ROTATION_H
