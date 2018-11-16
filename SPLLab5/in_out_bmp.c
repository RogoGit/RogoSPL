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

enum read_error_code read_bmp(char const* filename, struct image* input_image){

    if (filename == NULL){
        return READ_FILENAME_NOT_FOUND;
    }
    FILE* input = fopen(filename, "rb");
    if (input == NULL){
        return READ_FILE_NOT_FOUND;
    }
    struct bmp_header header;
    fread(&header, 1, sizeof(header), input);
    if (header.bfType == 0){
        return READ_INVALID_HEADER;
    }
    uint8_t* data = (uint8_t *)malloc(header.biSizeImage);
    fseek(input, header.bOffBits, SEEK_SET);
    fread(data, 1, header.biSizeImage, input);
    if (data == NULL){
        return READ_INVALID_BITS;
    }
    if (input_image == NULL) {
        input_image = (struct image*)malloc(sizeof(struct image));
    }
    input_image->data = (struct pixel*)malloc(header.biHeight * header.biWidth * sizeof(struct pixel));

    int padding = header.biWidth % 4;
    for (uint32_t i = 0; i < header.biHeight; ++i){
        for (uint32_t j = 0; j < header.biWidth; ++j){
            *(input_image->data + i*header.biWidth + j) = *(struct pixel*)(((uint8_t*)data) + sizeof(struct pixel)*(i*header.biWidth + j) + padding*i);
        }
    }
    input_image->height = header.biHeight;
    input_image->width = header.biWidth;
    fclose(input);
    return READ_OK;
}



enum write_error_code write_bmp(char const* filename, struct image const* image){

    if (image == NULL){
        return WRITE_IMAGE_NOT_FOUND;
    }
    if (filename == NULL){
        return WRITE_FILENAME_NOT_FOUND;
    }
 ////
    int padding = image->width % 4;
    struct bmp_header* header = create_header(image,padding);
    uint32_t i, j;
    struct image* new_image = (struct image*)malloc(sizeof(struct image));
    new_image->width = image->width+padding;
    new_image->height = image->height;
    new_image->data = (struct pixel*)calloc(1, new_image->height * new_image->width * sizeof(struct pixel));

    for (i = 0; i < new_image->height; ++i){
        for (j = 0; j < new_image->width; ++j){
            if(j < new_image->width - padding) {
                *(new_image->data + i * new_image->width + j) = *(image->data + i * image->width + j);
            }
        }
    }

    FILE* output = fopen(filename, "wb+");
    if (output == NULL){
        return WRITE_ERROR;
    }


    fwrite(header, 1, sizeof(struct bmp_header), output);
    fwrite(image->data, 1, new_image->height * new_image->width * sizeof(struct pixel), output);
    fclose(output);
    return WRITE_OK;
}

