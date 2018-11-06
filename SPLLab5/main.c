#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_pic_struct.h"
#include "rotation.h"
#include "in_out_bmp.h"

int main() {

    struct image* input_image = (struct image*)malloc(sizeof(struct image));
    printf("%zu",sizeof(&input_image));
    switch (read_pic("hqdefault.bmp", input_image)) {
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
    printf("%zu",sizeof(&input_image));
    struct image* output_image = rotate(input_image);
    switch (write_pic("out.bmp", output_image)){
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
    printf("Я записаль");
    return 0;
}