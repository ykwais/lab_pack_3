cmake_minimum_required(VERSION 3.23)
project(lab_4_7 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab_4_7 main.c funcs.c helper.h)
add_executable(test test.c)
