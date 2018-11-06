//
// Created by dell on 06.11.18.
//

#ifndef SPLLAB5_IN_OUT_BMP_H
#define SPLLAB5_IN_OUT_BMP_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "bmp_pic_struct.h"

 enum read_error_code {
    READ_OK = 0,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,

    READ_FILE_NOT_FOUND
    };

 enum  write_error_code {
    WRITE_OK = 0,
    WRITE_ERROR
};

enum write_error_code write_pic (char const* filename, struct image const* image);
enum read_error_code read_pic (char const* filename, struct image* inp_pic);
struct bmp_header* create_pic_header(struct image* pic,uint64_t padd);

#endif //SPLLAB5_IN_OUT_BMP_H
