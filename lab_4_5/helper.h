
#ifndef LAB_4_5_HELPER_H
#define LAB_4_5_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

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
    last_str,
    invalid_value,
    overflow,
    division_by_zero
}state;

typedef struct Stack_node{
    char symbol;
    struct Stack_node* next;
}Stack_node;

typedef struct Stack{
    Stack_node* top;
    Stack_node* bottom;
}Stack;

typedef struct Val_node{
    long long int value;
    struct Val_node* next;
}Val_node;

typedef struct Val_stack{
    Val_node* top;
    Val_node* bottom;
}Val_stack;

void print_state(state st, char* filename, FILE* file, const int* count, char* infix, char* postfix, const long long int* result);
int priority_of_symbol(char symbol);
char* read_line(FILE* file, state* stat);
state push(Stack* stack, char symbol);
state pop(Stack* stack);
void free_stack(Stack* stack);
state infix_to_postfix(char* str, char** postfix);
bool check_brackets(char* str);
state add_symbol_in_array_char(char** array, int* size, int* buf, char symbol);
char* generate_new_filename(char* filename);
int getNumberLength(long long int number);
state stringToLongLong(const char* str, long long int* res);
long long int bpow(long long int a, long long int b);
state calculate_postfix(char* postfix, long long int* result);
state push_val(Val_stack* stack, long long int val);
state pop_val(Val_stack* stack);
void free_val_stack(Val_stack* stack);

#endif //LAB_4_5_HELPER_H
