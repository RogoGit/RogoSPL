#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rotation.h"
#include "in_out_bmp.h"

int main() {

    struct image* inp_image = (struct image*)malloc(sizeof(struct image));

    struct image* inp1 = (struct image*)malloc(sizeof(struct image));
    read_picture("lain_guy-1x1.bmp",inp1);

    struct image* inp2 = (struct image*)malloc(sizeof(struct image));
    read_picture("lain_guy-1x2 .bmp",inp2);

    struct image* inp3 = (struct image*)malloc(sizeof(struct image));
    read_picture("lain_guy-2x1.bmp",inp3);

    struct image* inp4 = (struct image*)malloc(sizeof(struct image));
    read_picture("lain_guy-3k.bmp",inp4);

    struct image* out1 = rotate(inp1);
    struct image* out2 = rotate(inp2);
    struct image* out3 = rotate(inp3);
    struct image* out4 = rotate(inp4);

    write_picture("out11.bmp", out1);
    write_picture("out12.bmp", out2);
    write_picture("out21.bmp", out3);
    write_picture("out3k.bmp", out4);


    // hqdefault.bmp  osenie-vodopadu.bmp
    switch (read_picture("hqdefault.bmp", inp_image)) {

        case READ_INVALID_BITS: {
            puts("Некорректные данные для чтения");
            break;
        }
        case READ_INVALID_HEADER: {
            puts("Неверный заголовок файла");
            break;
        }
        case READ_OK:{
           puts("Изображение успешно принято");
            break;
        }
        case READ_FILE_NOT_FOUND: {
            puts("Не удаётся открыть файл для чтения");
            break;
        }
        default: {
            puts("Непредвиденная ошибка чтения");
            break;
        }
    }
   //struct image* out_image = inp_image;
   struct image* out_image = rotate(inp_image);

    switch (write_picture("out.bmp", out_image)){

        case WRITE_FILENAME_NOT_FOUND: {
            puts("Файл для записи не найден");
            break;
        }
        case WRITE_IMAGE_NOT_FOUND: {
            puts("Записывать нечего");
            break;
        }
        case WRITE_ERROR: {
            puts("Ошибка записи");
            break;
        }
        case WRITE_OK: {
            puts("Изображение записано");
            break;
        }
        default: {
            puts("Непредвиденная ошибка записи");
            break;
        }
    }
    return 0;
}
