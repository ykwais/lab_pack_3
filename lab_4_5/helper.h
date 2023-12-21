
#ifndef LAB_4_5_HELPER_H
#define LAB_4_5_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum{
    invalid_argument,
    well,
    empty_file,
    memory_problem,
    invalid_symbol,
    problem_with_opening_file,
    wrong_brackets,
    not_1_string_in_file,
    wrong_operation,
    last_str
}state;

typedef struct Stack_node{
    char symbol;
    struct Stack_node* next;
}Stack_node;

typedef struct Stack{
    Stack_node* top;
    Stack_node* bottom;
}Stack;

void print_state(state st, char* filename, FILE* file, const int* count, char* infix, char* postfix);
int priority_of_symbol(char symbol);
char* read_line(FILE* file, state* stat);
state push(Stack* stack, char symbol);
state pop(Stack* stack);
void free_stack(Stack* stack);
state infix_to_postfix(char* str, char** postfix);
bool check_brackets(char* str);
state add_symbol_in_array_char(char** array, int* size, int* buf, char symbol);
char* generate_new_filename(char* filename);


#endif //LAB_4_5_HELPER_H
