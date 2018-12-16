#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BMP_IO.h"

int main() {

    struct image* inp_image = (struct image*)malloc(sizeof(struct image));

    switch ( read_picture("lain_guy-3k.bmp",inp_image)) {

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
    struct image* out_image = inp_image;

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
