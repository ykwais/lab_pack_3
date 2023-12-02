
#ifndef LAB_3_7_HELPER_H
#define LAB_3_7_HELPER_H
#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>
#define EPS 1e-6



typedef struct Liver{
    char* surname;
    char* name;
    char* father_name;
    char* birth_day;
    char male;
    long double average_income;
}Liver;

typedef struct node{
    Liver* liver;
    struct node* next;
}node;

typedef struct list{
    node* in_head;
}list;

typedef enum input_status_code{
    is_well,
    is_empty_file,
    is_problem_open_file,
    is_invalid_string,
    is_mem_problem,
    is_problem_with_token,
    is_empty_str,
    is_wrong_amount_lex_in_str,
    is_problem_with_male,
    is_exist_liver
}input_state;

typedef enum status{
    well,
    meme_problem,
    invalid,
    empty_str,
    last_str,
    none
}state;

typedef enum type{
    delete,
    add,
    modification
}type;

typedef struct stack_node{
    type tp;
    node* pnode;
    node* current_in_list;
}stack_node;

typedef struct undo_stack{
    stack_node* nodes;
    int size;
    int buf;
}undo_stack;



char* read_line(state* stat, FILE* file);
int count_tokens(char *str, const char *delim);
void print_liver(Liver* liver, FILE* file);
input_state parsing_input_data(char* filename, list* lst);
bool is_latin_str(char* str);
state add_node(list* lst, Liver* liver);
node* create_node(Liver* liver);
node* find_same_liver(list* lst, Liver* liver);
void free_list(list* lst);
void print_list(list* lst, FILE* file);
time_t data_compare(char* str1, char* str2);
input_state interactive(list* lst, undo_stack* stack);
Liver* get_liver_stdin(state* stat);
void free_liver(Liver* liver);
void clear_input_buffer(FILE* file);
state add_undo_stack(undo_stack* stack, node* prev, type tp, node* current_in_list);
void delete_stack(undo_stack* stack);
Liver* copy_liver(Liver* searchable);
void print_undo_stack(undo_stack* stack);
void delete_node_list(list* lst, node* tmp);


#endif //LAB_3_7_HELPER_H
