#include <stdlib.h>
#include <stdio.h>
#include "in_out_bmp.h"
#include "bmp_pic_struct.h"

struct bmp_header* create_header(struct image const* pic, int padding) {

    struct bmp_header* header = (struct bmp_header*)malloc(sizeof(struct bmp_header));

    header->bfType = 19778;
    header->bfileSize = pic->width * pic->height * sizeof(struct pixel) + sizeof(header);
    header->bfReserved = 0;
    header->bOffBits = sizeof(struct bmp_header);
    header->biSize = 40;
    header->biPlanes = 0;
    header->biBitCount = 24;
    header->biCompression = 0;
    header->biSizeImage = pic->width * pic->height * sizeof(struct pixel);
    header->biXPelsPerMeter = 2835;
    header->biYPelsPerMeter = 2835;
    header->biClrUsed = 0;
    header->biClrImportant = 0;
    header->biWidth = pic->width - padding;
    header->biHeight = pic->height;
    return header;
}

enum read_error_code read_picture(char const* filename, struct image* input_bmp){

    int row;
    int col;

    struct bmp_header header;
    FILE* input_file = fopen(filename, "rb+");

    if (filename == NULL || input_file == NULL){ return READ_FILE_NOT_FOUND; }

    fread(&header, 1, sizeof(header), input_file);

    if (header.bfType == 0){ return READ_INVALID_HEADER; }

    uint8_t* data = (uint8_t *)malloc(header.biSizeImage);
    fseek(input_file, header.bOffBits, SEEK_SET);
    fread(data, 1, header.biSizeImage, input_file);

    if (data == NULL){ return READ_INVALID_BITS; }

    if (input_bmp == NULL) {
        input_bmp = (struct image*)malloc(sizeof(struct image));
    }

    input_bmp->data = (struct pixel*)malloc(header.biHeight * header.biWidth * sizeof(struct pixel));
    input_bmp->height = header.biHeight;
    input_bmp->width = header.biWidth;
    int padding = header.biWidth % 4;

    for (row = 0; row < header.biHeight; row++){
        for (col = 0; col < header.biWidth; col++){
            input_bmp->data[row*header.biWidth + col] =
                    *(struct pixel*)((data) + sizeof(struct pixel)*(row*header.biWidth + col) + padding*row);
        }
    }


    fclose(input_file);

    return READ_OK;
}



enum write_error_code write_picture(char const* filename, struct image const* out_bmp){

    if (filename == NULL){ return WRITE_FILENAME_NOT_FOUND; }
    if (out_bmp == NULL){ return WRITE_IMAGE_NOT_FOUND; }

    int padding = out_bmp->width % 4;
    int row;
    int col;

    struct bmp_header* header = create_header(out_bmp,padding);

    struct image* new_image = (struct image*)malloc(sizeof(struct image));
    new_image->height = out_bmp->height;
    new_image->width = out_bmp->width+padding;
    new_image->data = (struct pixel*)calloc(1, new_image->height * new_image->width * sizeof(struct pixel));

    for (row = 0; row < new_image->height; row++){
        for (col = 0; col < new_image->width; col++){
            if(col < new_image->width - padding) {
                *(new_image->data + row * new_image->width + col) = *(out_bmp->data + col * out_bmp->width + row);
            }
        }
    }


    FILE* output = fopen(filename, "wb+");
    if (output == NULL){ return WRITE_ERROR; }

    fwrite(header, 1, sizeof(struct bmp_header), output);
    fwrite(out_bmp->data, 1, new_image->height * new_image->width * sizeof(struct pixel), output);
    fclose(output);
    return WRITE_OK;
}

