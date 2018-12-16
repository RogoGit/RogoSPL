#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdint.h>
#include "BMP_IO.h"
#include "sepia_filter .h"

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

    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r );
    start = r.ru_utime;
   // for( uint64_t i = 0; i < 100000000; i++ );
    sepia_c_inplace(inp_image);
    getrusage(RUSAGE_SELF, &r );
    end = r.ru_utime;
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) +
               end.tv_usec - start.tv_usec;
    printf( "Time elapsed in microseconds: %ld\n", res );

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
