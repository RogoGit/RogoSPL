//
// Created by dell on 06.11.18.
//

#include "in_out_bmp.h"
#include "bmp_pic_struct.h"

struct bmp_header* create_pic_header(struct image* pic,uint64_t padd) {
    struct bmp_header* header = (struct bmp_header*)malloc(sizeof(struct bmp_header));
    header->bfType = 19778;
    header->bfileSize = pic->width * pic->height * sizeof(struct pixel) + sizeof(header);
    header->bfReserved = 0;
    header->OffSet = sizeof(struct bmp_header);
    header->biSize = 40;
    header->biPlanes = 0;
    header->biBitCount = 24;
    header->biCompression = 0;
    header->biSizeImage = pic->width * pic->height * sizeof(struct pixel);
    header->biXPelsPerMeter = 2835;
    header->biYPelsPerMeter = 2835;
    header->biClrUsed = 0;
    header->biClrImportant = 0;
    header->biWidth = pic->width - padd;
    header->biHeight = pic->height;
    return header;
}

enum read_error_code read_pic (char const* filename, struct image* inp_pic){

    uint32_t row;
    uint32_t col;

    FILE* in = fopen(filename, "rb");
    if ((filename == NULL) || (in == NULL)) { return READ_FILE_NOT_FOUND; }

    struct bmp_header header;
    fread(&header, 1, sizeof(header), in);
    u_int64_t padd = header.biWidth % 4;

    if (header.bfType != 19778){ return READ_INVALID_HEADER; }

    uint8_t* pixel_data = (uint8_t *)malloc(header.biSizeImage);
    fseek(in, header.OffSet, SEEK_SET);
    fread(pixel_data, 1, header.biSizeImage, in);

    if (pixel_data == NULL) { return READ_INVALID_BITS; }
    //if (inp_pic == NULL) {
    inp_pic = (struct image*)malloc(sizeof(struct image));
    //}
    inp_pic->height = header.biHeight;
    inp_pic->width = header.biWidth;
    inp_pic->data = (struct pixel*)malloc(header.biHeight * header.biWidth * sizeof(struct pixel));

    for (row = 0; row < header.biHeight; row++){
        for (col = 0; col < header.biWidth; col++){
            inp_pic->data[row*header.biWidth + col] =
                    *(struct pixel*)((pixel_data) + sizeof(struct pixel)*(row*header.biWidth + col) + padd*row);
        }
    }
    fclose(in);
    return READ_OK;
}


enum write_error_code write_pic (char const* filename, struct image const* image) {

    uint64_t padd = image->width % 4;
    uint32_t row;
    uint32_t col;

    if ((image == NULL)||(filename == NULL)) return WRITE_ERROR;

    struct image* new_pic = (struct image*)malloc(sizeof(struct image));
    new_pic->width = image->width+padd;
    new_pic->height = image->height;
    new_pic->data = (struct pixel*)calloc(1, new_pic->height * new_pic->width * sizeof(struct pixel));

    for (row = 0; row < new_pic->height; row++){
        for (col = 0; col < new_pic->width; col++){
            if(col < new_pic->width - padd) {
                new_pic->data[row * new_pic->width + col] = image->data[row * image->width + col];
            }
        }
    }

    FILE* out = fopen(filename, "wb+");
    if (out == NULL){ return WRITE_ERROR; }
    //
//struct bmp_header* header = create_pic_header(new_pic,padd);
    struct bmp_header* header = (struct bmp_header*)malloc(sizeof(struct bmp_header));
    fwrite(header, 1, sizeof(struct bmp_header), out);

    header->bfType = 19778;
    header->bfileSize = new_pic->width * new_pic->height * sizeof(struct pixel) + sizeof(header);
    header->bfReserved = 0;
    header->OffSet = sizeof(struct bmp_header);
    header->biSize = 40;
    header->biPlanes = 0;
    header->biBitCount = 24;
    header->biCompression = 0;
    header->biSizeImage = new_pic->width * new_pic->height * sizeof(struct pixel);
    header->biXPelsPerMeter = 2835;
    header->biYPelsPerMeter = 2835;
    header->biClrUsed = 0;
    header->biClrImportant = 0;
    header->biWidth = new_pic->width - padd;
    header->biHeight = new_pic->height;

    fwrite(image->data, 1, new_pic->height * new_pic->width * sizeof(struct pixel), out);
    fclose(out);
    return WRITE_OK;
}