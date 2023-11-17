
#ifndef LAB_3_10_HELPER_H
#define LAB_3_10_HELPER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node{
    char* arg;
    struct node* kid;
    struct node* bro;
}node;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_empty_file,
    ic_wrong_count_args,
    ic_same_file
}input_status;

typedef enum state{
    mem_problem,
    st_well
}state;

input_status check_input(int argc, char** argv);
node* get_root(const char* string,state* stat);
node* create_node(char* str);
void print_tree(node* root, FILE* output_file, int depth);
char* get_lexemma(const char* string, int* counter, const int* calcul);
void free_tree(node* root);


#endif //LAB_3_10_HELPER_H
