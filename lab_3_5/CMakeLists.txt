cmake_minimum_required(VERSION 3.22)
project(lab_3_5 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab_3_5 main.c helper.h funcs.c)
add_executable(re re_main.c helper_2.h funcs_2.c)