cmake_minimum_required(VERSION 3.22)
project(lab_3_9 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab_3_9 main.c tree.c helper.h funcs.c)
add_executable(test test.c)