

#ifndef LAB_3_5_HELPER_2_H
#define LAB_3_5_HELPER_2_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned int ui;

typedef struct{
    ui id;
    char* name;
    char* surname;
    char* group;
    unsigned char* marks;
}Student;

typedef enum status{
    well,
    meme_problem,
    invalid,
    empty_str,
    last_str,
    none
}state;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_wrong_amount_lex,
    ic_mem_problem,
    ic_the_same_id,
    ic_empty_file,
    ic_empty_str

}input_status;

char* read_line(state* stat, FILE* file);
int count_tokens(char *str, const char *delim);
input_status collect_data(char* filename, Student** list, int* stud_count, int* buf);
void delete_student(Student* student);
char* create_string(char* str);
bool is_latin_str(char* str);
void free_students(Student** lst, const int *count);
void print_student(Student *student, FILE* file);

#endif //LAB_3_5_HELPER_2_H
