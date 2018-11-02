//
// Created by dell on 02.11.18.
//
#include <stdint.h>
#ifndef SPLLAB5_BMP_PIC_STRUCT_H
#define SPLLAB5_BMP_PIC_STRUCT_H

struct __attribute__((packed)) bmp_header {

    uint16_t bfType;
    uint16_t biPlanes;
    uint16_t biBitCount;

    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t OffSet;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;

};

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

#endif //SPLLAB5_BMP_PIC_STRUCT_H
