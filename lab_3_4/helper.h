

#ifndef LAB_3_4_HELPER_H
#define LAB_3_4_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef enum status{
    well,
    meme_problem,
    invalid
}state;

typedef struct{
    int size;//фактически записано
    char* arg;
}string;

string* create_string(const char* str, state* stat);
void delete_string(string* in);
int equal(const string* left, const string* right, state* stat);
int order_relation_string(const string* left, const string* right, state* stat);
state copy_to_exist(string* from, string* to);
string* copy_to_new_string(string* from, state* stat);
state cnt_string(string* first, string* second);

#endif //LAB_3_4_HELPER_H
