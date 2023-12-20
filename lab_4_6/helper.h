
#ifndef LAB_4_6_HELPER_H
#define LAB_4_6_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 12
#define MIN_LEN 1
#define TXT 4

//(~((A&B)|c) & (1 | 0)) & ((A|B) & (A | 1) = (A!(y->B)  & (1 | 0)))

typedef enum{
    invalid_argument,
    well,
    empty_file,
    memory_problem,
    invalid_symbol,
    problem_with_opening_file,
    wrong_brackets,
    not_1_string_in_file,
    wrong_operation
}state;

typedef struct Stack_node{
    char symbol;
    struct Stack_node* next;
}Stack_node;

typedef struct Stack{
    Stack_node* top;
    Stack_node* bottom;
}Stack;

typedef struct Node{
    char symbol;
    struct Node* left;
    struct Node* right;
}Node;

typedef struct Tree{
    Node* root;
}Tree;

typedef struct Node_stack{
    Node* node;
    struct Node_stack* next;
}Node_stack;

typedef struct Tree_stack{
    Node_stack* top;
    Node_stack* bottom;
}Tree_stack;



void print_state(state st);
bool check_brackets(char* str);
char* read_line(FILE* file);
state pop(Stack* stack);
state push(Stack* stack, char symbol);
int priority_of_symbol(char symbol);
state infix_to_postfix(char* str, char** postfix);
void free_stack(Stack* stack);
state get_postfix(char* filename, char** postix );
state push_tree_stack(Tree_stack* stack, Node* node);
state pop_tree_stack(Tree_stack* stack);
state build_tree(char* postfix, Tree* tree);
void print_tree(Node* root, int level);
void free_tree(Node* root);
int count_unique_symbols(char* postfix, char** unique_symbols);
state print_table(Node* root, int count, char* unique_symbols);
char* generate_filename();
state pop_tree_stack_full(Tree_stack* stack);
int get_value_from_tree(Node* root, char* unique_symbols, int count, int* val);
bool not_used_symbol(const char* unique_symbols, char symbol, int count);
bool is_operator(char symbol);
Node* create_node_tree(char symbol);
void free_tree_stack(Tree_stack* stack);
void free_tree_stack_full(Tree_stack* stack);


#endif //LAB_4_6_HELPER_H
