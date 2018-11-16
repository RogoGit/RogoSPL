#include <stdio.h>
#include <stdlib.h>
#include "rotation.h"
#include "in_out_bmp.h"
#include "big_files.h"

int main() {

    /*rotate_bmp("hqdefault.bmp", "out.bmp");
    image_bmp_fused_rotate_90cw("hqdefault.bmp", "out2.bmp");
    */

    struct image* inp_image = (struct image*)malloc(sizeof(struct image));
    switch (read_picture("hqdefault.bmp", inp_image)) {

        case READ_INVALID_BITS: {
            printf("Проблемы с данными.\n");
            break;
        }
        case READ_INVALID_HEADER: {
            printf("Проблемы с заголовком.\n");
            break;
        }
        case READ_OK:{
            printf("Изображение получено.\n");
            break;
        }
        default: {
            printf("Вообще что-то не так.\n");
            break;
        }
    }

    struct image* out_image = rotate(inp_image);
    switch (write_picture("out.bmp", out_image)){
        case WRITE_IMAGE_NOT_FOUND: {
            printf("Изображение для записи не найдено.\n");
            break;
        }
        case WRITE_FILENAME_NOT_FOUND: {
            printf("А тут как-то неправильно передается filename.\n");
            break;
        }
        case WRITE_ERROR: {
            printf("Незнаю, как такое получилось, но тут ошибка при открытии файла.\n");
            break;
        }
        case WRITE_OK: {
            printf("Я записаль:3\n");
            break;
        }
        default: {
            printf("Совсем что-то не так пошло.\n");
            break;
        }
    }
    return 0;
}