
#ifndef LAB_4_1_HELPER_H
#define LAB_4_1_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#define HASHSIZE 4

typedef enum state{
    well,
    meme_problem,
    empty_file,
    same_file,
    wrong_amount_of_arguments,
    problem_with_file_opening,
    last_str,
    invalid_data
}state;

typedef struct Node{
    char* key;
    char* value;
    struct Node* next;
}Node;

typedef struct HashTable{
    unsigned int size;
    Node** table;
}HashTable;


void print_state(state st);
HashTable* create_hashtable(unsigned int size);
void remove_hashtable(HashTable* ht);
char* read_line(state* stat, FILE* file);
char* read_define_line(state* st, char* ptr);
int count_tokens(char *str, const char *delim);
state get_hash(char* key, HashTable* ht, unsigned long long* result);
state insert_in_ht(HashTable* ht, char* key, char* value);
bool is_correct_define(char* str);
state resize_ht(HashTable** ht, unsigned int new_size);
state balance_insert(HashTable** ht, char* key, char* value);
state file_work(char* input_name, char* out_name, HashTable** ht);
state change_string(char **string, const char *old, const char *new);
//state change_string(char** str, const char* old, const char* new);


#endif //LAB_4_1_HELPER_H
