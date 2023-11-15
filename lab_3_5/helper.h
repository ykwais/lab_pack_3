
#ifndef LAB_3_5_HELPER_H
#define LAB_3_5_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define COUNT_STR 2
#define MAX_LENGTH_STR 100

typedef unsigned int ui;

typedef struct{
    ui id;
    char name[100];
    char surname[100];
    char group[50];
    unsigned char* marks;
}Student;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_wrong_amount_lex,
    ic_mem_problem,
    ic_the_same_id,
    ic_empty_file

}input_status;

typedef enum lexemma{
    too_long_lexemma,
    norm_lexemma
} size_lex;



input_status collect_data(char* filename, Student** list, size_lex* len_lex, int* stud_count);
int compare_by_id(const void *a, const void *b);
int compare_by_surname(const void *a, const void *b);
int compare_by_name(const void *a, const void *b);
int compare_by_group(const void *a, const void *b);
void print_student(Student *student, FILE* file);
void print_average_grades(Student *students, int count, FILE *file);

#endif //LAB_3_5_HELPER_H
