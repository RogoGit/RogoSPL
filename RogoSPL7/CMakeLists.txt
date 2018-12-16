cmake_minimum_required(VERSION 3.12)
project(RogoSPL7 C)

set(CMAKE_C_STANDARD 11)

add_executable(RogoSPL7 main.c BMP_Struct.h BMP_IO.c BMP_IO.h)