#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_pic_struct.h"

struct image* rotate(const struct image* pic) {
    uint32_t row;
    uint32_t col;

    struct image* rotated = (struct image*)malloc( pic->width * pic->height * sizeof(struct image));
    rotated->data = (struct pixel*)malloc( pic->height * pic->width* sizeof(struct pixel));
    rotated->width = pic->height;
    rotated->height = pic->width;

    for (row = 0;  row < pic->height; row++){
        for (col = 0; col < pic->width; col++){
            rotated->data [col * pic->height + row] = rotated->data [row * pic->width + col];
        }
    }
    return rotated;

}

int main() {
    struct bmp_header header;
    FILE *f = fopen("hqdefault.bmp", "rb");
    fread(&header, 1, sizeof(header), f);

    struct pixel* data = (struct pixel*)malloc(header.biSizeImage);
    fseek(f, header.OffSet, SEEK_SET);
    fread(data, 1, header.biSizeImage, f);
    fclose(f);

    struct image* pic = (struct image*)malloc(sizeof(struct image));
    pic->height = header.biHeight;
    pic->width = header.biWidth;
    pic->data = data;

    //printf("Magic: %c%c\n", header.magic_number[0], header.magic_number[1]);
    return 0;
}