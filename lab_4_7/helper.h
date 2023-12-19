
#ifndef LAB_4_7_HELPER_H
#define LAB_4_7_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum state
{
    well,
    invalid_input,
    error_open,
    div_zero,
    invalid_argument,
    meme_problem,
    uninit_var,
    conv_error,
    no_var,
    empty_file
} state;

typedef enum var_state
{
    print_all,
    print_one,
    print_val
}var_state;

typedef enum operation_type{
    add,
    sub,
    mul,
    division,
    mod,
    eql,
    undef
}operation_type;

typedef struct {
    char* var_name;
    long long int value;
}memory_cell;

typedef struct{
    memory_cell* cells;
    int size;
    int buf;
}memory;


void print_status_codes(state status);
state init_memory(memory* storage);
void free_memory(memory* storage);
bool is_operation(char ch);
state get_instruction(FILE* file, char** name, char** operation, char** first_arg, char** second_arg);
void print_var(var_state st, memory_cell* cell, memory* storage, const long long int* value);
memory_cell* find_var(memory* storage, char* var_name);
state string_to_ll(char* str, long long int* res);
int compare_var_name(const void* first_name, const void* second_name);
state add_to_memory(memory* storage, char* var_name, long long int value);
operation_type get_operation(char symb);
state make_operation(long long int* res, long long int first, long long int second, operation_type operation);
state parsing_file(FILE* file, memory* storage);
void free_4_arr(char** first_arg, char** second_arg, char** operation, char** var_name);

#endif //LAB_4_7_HELPER_H
