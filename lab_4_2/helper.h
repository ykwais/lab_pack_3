
#ifndef LAB_4_2_HELPER_H
#define LAB_4_2_HELPER_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <errno.h>


typedef enum
{
    well,
    probem_file_opening,
    file_wrote,
    file_read,
    same_file,
    mem_problem,
    invalid_comand,
    invalid_index,
    invalid_range,
    empty_array,
    not_existed_array,
    exist_array,
    int_overflow,
    invalid_format,
    invalid_count_argc,
    empty_file
} state;

typedef struct Massive {
    char name;
    int* data;
    int size;
    int buf;
} Massive;

state free_array(Massive** array);
state create_new_array(Massive** arr, char name, int buf);
void free_all_arrays(Massive* array_storage[], int size);
state string_to_int(char* str, int* value);
state get_arr_from_file(char* filename, Massive* arr);
state save_to_file(char* filename, Massive* arr);
state parse_file(char* filename);
state fill_randomly(Massive* arr, int lb, int rb, int count);
state concatination_arrays(Massive* arr_first, Massive* arr_second);
state remove_some_elem_from_array(Massive* arr, int start, int count);
state copy_part_arr(Massive* first, Massive* second, int start, int end);
state sort_array(Massive* arr, char type);
int compare_minus(const void* a, const void* b);
int compare_plus(const void* a, const void* b);
state shuffle(Massive* arr);
state get_max_array(Massive* arr, int* max, int* max_index);
state get_min_array(Massive* arr, int* min, int* min_index);
state get_most_often(Massive* arr, int* most_often, int* count);
state get_average_from_array(Massive* arr, double* average);
state get_max_otklonenie(Massive* arr, double* max_otklonenie, int* position, int* number);
void print_stats_array(Massive* arr);
state print_one_elem(Massive* arr, int index);
state print_range(Massive* arr, int start, int end);
state print_all(Massive* arr);
void print_status(state status);
#endif //LAB_4_2_HELPER_H
