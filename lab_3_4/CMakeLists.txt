cmake_minimum_required(VERSION 3.23)
project(lab_3_4 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab_3_4 main.c funcs.c helper.h)
add_executable(mail mail.c funcs.c helper.h)
