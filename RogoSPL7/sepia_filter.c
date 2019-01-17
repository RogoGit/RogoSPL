//
// Created by dell on 17.01.19.
//

//
// Created by dell on 16.12.18.
//

#include <stdint.h>
#include "BMP_Struct.h"
#include "BMP_IO.h"
#include "sepia_filter .h"

static unsigned char sat(uint64_t x) {
    if (x < 256) return x; return 255;
}
static void sepia_one( struct pixel* const pixel ) {
    static const float c[3][3] = {
            { .393f, .769f, .189f },
            { .349f, .686f, .168f },
            { .272f, .543f, .131f } };

    struct pixel const old = *pixel;
    pixel->r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
    pixel->g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
    pixel->b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
}
void sepia_c_inplace( struct image* img ) {
    uint32_t x,y;
    for( y = 0; y < img->height; y++ )
        for( x = 0; x < img->width; x++ )
            sepia_one( img->data + y * img->width + x); //
}

void sepia_filter_sse(struct pixel* pixel, uint32_t size);

void sepia_sse_inplace(struct image *img) {
    if (img->height * img->width < 4){
        for (int i = 0; i < img->height * img->width; ++i){
            sepia_one(img->data + i);
        }
        return;
    }
    sepia_filter_sse(img->data, img->height * img->width - (img->height * img->width) % 4);
    for (int i  = img->height * img->width - (img->height * img->width) % 4; i < img->height * img->width; ++i){
        sepia_one(img->data + i);
    }
}