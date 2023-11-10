
#ifndef LAB_3_3_HELPER_H
#define LAB_3_3_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define MAX_LENGTH_STR 100
#define COUNT_STR 2

typedef unsigned int ui;

typedef struct
{
    ui id;
    char name[MAX_LENGTH_STR];
    char sur_name[MAX_LENGTH_STR];
    double money;
} Employee;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_wrong_amount_lex,
    ic_mem_problem

}input_status;

typedef enum lexemma{
    too_long_lexemma,
    norm_lexemma
} size_lex;

typedef enum valid_status{
    vs_well,
    vs_problem_input_number,
    vs_problem_input
}validate;

input_status collect_data(char* filename, Employee** list, size_lex* len_lex, int* stud_count);
validate analysis(int argc, char** argv, char* flag);
int compare_right(const void *a, const void *b);
int compare_left(const void *a, const void *b);
void print_emps(Employee** list, int* count, FILE* file);

#endif //LAB_3_3_HELPER_H
