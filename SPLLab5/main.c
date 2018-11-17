#include <stdio.h>
#include <stdlib.h>
#include "rotation.h"
#include "in_out_bmp.h"
#include "big_files.h"

int main() {

    struct image* inp_image = (struct image*)malloc(sizeof(struct image));
//handle_big_file("Lights.bmp","out2.bmp"); // hqdefault.bmp  osenie-vodopadu.bmp
    switch (read_picture("Lights.bmp", inp_image)) {

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

/*rotate_bmp("hqdefault.bmp", "out.bmp");
    image_bmp_fused_rotate_90cw("hqdefault.bmp", "out2.bmp");
    */