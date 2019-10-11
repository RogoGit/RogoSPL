cmake_minimum_required(VERSION 3.12)
project(RogoShell C)

set(CMAKE_C_STANDARD 99)

add_executable(RogoShell main.c UI.c UI.h commands.c commands.h util.c util.h macro.h)