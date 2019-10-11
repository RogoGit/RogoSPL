cmake_minimum_required(VERSION 3.12)
project(RogSPLLab4 C)

set(CMAKE_C_STANDARD 90)

add_executable(RogSPLLab4 main.c LinkedList.c LinkedList.h FunctionPack.h HighOrderFunc.c cmake-build-debug/Lab4In.txt cmake-build-debug/Lab4Out.txt)