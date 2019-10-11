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
    if (x < 256) return x;
    return 255;
}

struct rhombus_line {
    float k;
    float b;
};

struct rhombus_line *get_line(float x1, float y1, float x2, float y2) {
    struct rhombus_line *line = malloc(sizeof(struct rhombus_line));
    line->k = (y2 - y1) / (x2 - x1);
    line->b = y1 - x1 * line->k;
    return line;
}

float get_res(float x, float k, float b) { return (k * x + b); }

static void sepia_one(struct pixel *const pixel) {
    static const float c[3][3] = {
            {.393f, .769f, .189f},
            {.349f, .686f, .168f},
            {.272f, .543f, .131f}};

    struct pixel const old = *pixel;
    pixel->r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
    pixel->g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
    pixel->b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
}

void sepia_c_inplace(struct image *img) {

    // struct rhombus_line* first_quadro_higher = get_line((float)img->width, (float)img->height/2, (float)img->width/2, 0);
    // struct rhombus_line* first_quadro_lower = get_line((float)(3*img->width/4), (float)img->height/2, (float)img->width/2, (float)img->height/4);

    struct rhombus_line *bottom_left_higher = get_line(0, (float) img->height / 2,
                                                       (float) img->width / 2, 0);
    struct rhombus_line *bottom_left_lower = get_line(0, (float) img->height / 4, (float) img->width / 4, 0);

    struct rhombus_line *bottom_right_higher = get_line((float) img->width / 2, 0, (float) img->width,
                                                        (float) img->height / 2);
    struct rhombus_line *bottom_right_lower = get_line((float) img->width / 4 * 3, 0, (float) img->width,
                                                       (float) img->height / 4);

    struct rhombus_line *top_left_higher = get_line(0, (float) img->height / 4 * 3,
                                                    (float) img->width / 4, (float) img->height);
    struct rhombus_line *top_left_lower = get_line(0, (float) img->height / 2,
                                                   (float) img->width / 2, (float) img->height);

    struct rhombus_line *top_right_higher = get_line((float) img->width / 4 * 3, (float) img->height,
                                                     (float) img->width, (float) img->height / 4 * 3);
    struct rhombus_line *top_right_lower = get_line((float) img->width / 2, (float) img->height,
                                                    (float) img->width, (float) img->height / 2);


    uint32_t x, y;
    for (y = 0; y < img->height; y++)
        for (x = 0; x < img->width; x++) {
            if ((x < img->width / 2) && (y < img->height / 2)) {
                if ((y > get_res(x, bottom_left_lower->k, bottom_left_lower->b)) &&
                    (y < get_res(x, bottom_left_higher->k, bottom_left_higher->b))) { //ok
                    sepia_one(img->data + y * img->width + x);
                }
            }
            if ((x > img->width / 2) && (y < img->height / 2)) {
                if ((y > get_res(x, bottom_right_lower->k, bottom_right_lower->b)) &&
                    (y < get_res(x, bottom_right_higher->k, bottom_right_higher->b))) {
                    sepia_one(img->data + y * img->width + x);
                }
            }
            if ((x < img->width / 2) && (y > img->height / 2)) {
                if ((y > get_res(x, top_left_lower->k, top_left_lower->b)) &&
                    (y < get_res(x, top_left_higher->k, top_left_higher->b))) {
                    sepia_one(img->data + y * img->width + x);
                }
            }
            if ((x > img->width / 2) && (y > img->height / 2)) {
                if ((y > get_res(x, top_right_lower->k, top_right_lower->b)) &&
                    (y < get_res(x, top_right_higher->k, top_right_higher->b))) {
                    sepia_one(img->data + y * img->width + x);
                }
            }
        }
}

void sepia_filter_sse(struct pixel *pixel, uint32_t size);

void sepia_sse_inplace(struct image *img) {
    if (img->height * img->width < 4) {
        for (int i = 0; i < img->height * img->width; ++i) {
            sepia_one(img->data + i);
        }
        return;
    }
    //sepia_filter_sse(img->data, img->height * img->width - (img->height * img->width) % 4);
    for (int i = img->height * img->width - (img->height * img->width) % 4; i < img->height * img->width; ++i) {
        sepia_one(img->data + i);
    }
}