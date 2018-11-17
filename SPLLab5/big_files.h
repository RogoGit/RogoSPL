#include <stdint.h>

#ifndef LAB5_MEMORY_MAPPING_AND_FRIENDS_H
#define LAB5_MEMORY_MAPPING_AND_FRIENDS_H

void rotate_bmp(char* input_filename, char* output_filename);

void image_bmp_fused_rotate_90cw(const char* src_path, const char* dst_path);

void handle_big_file(const char* inp, const char* out);

#endif //LAB5_MEMORY_MAPPING_AND_FRIENDS_H