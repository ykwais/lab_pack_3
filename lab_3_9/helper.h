
#ifndef LAB_3_9_HELPER_H
#define LAB_3_9_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

typedef enum state{
    well,
    meme_problem,
    empty_file,
    problem_with_file_open,
    empty_str
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

typedef struct arr_nodes{
    Node** nodes;
    int size;
    int buf;
}arr_nodes;

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
state interactive(Tree** tree, int* amount_saved, int* buffer, char*** array_saved_files);
char* read_line(state* stat, FILE* file);
void insert_arr(arr_nodes* arr, Node* curr, state* st);
void get_array(Node* root, arr_nodes* array, state* st);
int comp_occur(const void* a, const void* b);
int comp_word(const char* a, const char* b);
Node* get_min(Node* root);
Node* get_max(Node* root);
int depth_tree(Node* root);
void save_tree(Tree* tree, FILE* file);
void save_tree_node(Node* root, FILE* file);
#endif //LAB_3_9_HELPER_H
