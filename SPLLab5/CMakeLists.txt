cmake_minimum_required(VERSION 3.12)
project(SPLLab5 C)

set(CMAKE_C_STANDARD 11)

add_executable(SPLLab5 main.c bmp_pic_struct.h rotation.c rotation.h in_out_bmp.c in_out_bmp.h big_files.c big_files.h)