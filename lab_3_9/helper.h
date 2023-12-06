
#ifndef LAB_3_9_HELPER_H
#define LAB_3_9_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum state{
    well,
    meme_problem,
    empty_file,
    problem_with_file_open
}state;

typedef enum new_not{
    new,
    old
}new_old;

typedef struct Node{
    struct Node* left;
    struct Node* right;
    char* word;
    int counter;
}Node;

typedef struct Tree{
    Node* root;
}Tree;

typedef enum status_read_line{
    rl_well,
    rl_eof,
    rl_mem_problem
}rl_status;

Tree* init_tree();
Node* create_node(char* str);
void delete_node(Node* current_node);
void delete_tree(Node* root);
Node* add_node(Node* root, Node* new_node, new_old* st);
Node* find_node(Node* root, const char* str);
void print_node(Node* node);
void print_tree(Node* root, int depth);
void print_status(state st);
char* get_word(FILE* file, const char* separators, rl_status* stat);
state build_tree_with_file(Tree* tree, const char* filename, const char* separators);

#endif //LAB_3_9_HELPER_H
