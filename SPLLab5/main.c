#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_pic_struct.h"
#include "rotation.h"

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

    struct image* new_picture = rotate(pic);

    //printf("Magic: %c%c\n", header.magic_number[0], header.magic_number[1]);
    return 0;
}