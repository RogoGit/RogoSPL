#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "big_files.h"
#include "in_out_bmp.h"
#include "rotation.h"
#include "bmp_pic_struct.h"

    void handle_big_file(const char* inp, const char* out) {

        struct stat sourceStats;

        int sourceFile = open(inp, O_RDONLY);
        int destFile = open(out, O_RDWR | O_CREAT | O_TRUNC, (mode_t) 0644);

        fstat(sourceFile, &sourceStats);
        size_t sourceSize = sourceStats.st_size;

        uint8_t* sourceData = mmap(NULL, sourceSize, PROT_READ, MAP_SHARED, sourceFile, 0);
        struct bmp_header header = *((struct bmp_header*) sourceData);

        uint32_t width = header.biWidth;
        uint32_t height = header.biHeight;
        size_t destSize = header.bOffBits + width * height * sizeof(struct pixel) + compute_total_padding_rotated(&header);

        lseek(destFile, destSize - 1, SEEK_SET);
        write(destFile, "", 1);

        uint8_t* destData = mmap(NULL, destSize, PROT_READ | PROT_WRITE, MAP_SHARED, destFile, 0);

        rotate_right_padded(sourceData + header.bOffBits, destData + header.bOffBits, width, height);


        header.biHeight = width;
        header.biWidth = height;
        memcpy(destData, &header, sizeof(struct bmp_header));

        msync(destData, destSize, MS_SYNC);
        munmap(sourceData, sourceSize);
        munmap(destFile, sourceSize);

        close(sourceFile);
        close(destFile);
    }
