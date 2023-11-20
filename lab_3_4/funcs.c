#include "helper.h"

string* create_string(const char* str, state* stat)
{
    if(str == NULL || stat == NULL){
        *stat = invalid;
        return NULL;
    }
    string* new = (string*) malloc(sizeof(string));
    if(new == NULL){
        *stat = meme_problem;
        return NULL;
    }
    new->arg = NULL;
    new->size = 0;

    int len = (int)strlen(str);
    new->arg = (char*)malloc(sizeof(char)*(len+1));
    if(new->arg == NULL){
        *stat = meme_problem;
        delete_string(new);
        return NULL;
    }
    new->size = len;
    for(int i = 0; i < len; ++i)
    {
        new->arg[i] = str[i];
    }
    new->arg[len] = '\0';
    *stat = well;
    return new;
}


void delete_string(string* in)
{
    if(in == NULL)
    {
        return;
    }

    if(in->arg != NULL){
        free(in->arg);
    }

    free(in);

}

int equal(const string* left, const string* right, state* stat)
{
    if(left == NULL || right == NULL){
        *stat = invalid;
        return 0;
    }

    char* left_l = left->arg;
    char* right_r = right->arg;
    while(*left_l && *right_r)
    {
        if(*left_l != *right_r){
            *stat = well;
            return (unsigned char)*left_l - (unsigned char)*right_r;
        }
        left_l++;
        right_r++;
    }
    *stat = well;
    return (unsigned char)*left_l - (unsigned char)*right_r;
}


int order_relation_string(const string* left, const string* right, state* stat)
{
    if(left == NULL || right == NULL){
        *stat = invalid;
        return 0;
    }

    int res;

    res = left->size - right->size;
    if(res != 0){
        *stat = well;
        return res;
    }
    res = equal(left, right, stat);
    if(*stat != well){
        return 0;
    }
    *stat = well;
    return res;
}


state copy_to_exist(string* from, string* to)
{
    if(from == NULL || from->arg == NULL || to == NULL)
    {
        return invalid;
    }
    if(to->arg != NULL){
        free(to->arg);
        to->size = 0;
    }

    char* tmp = (char*)malloc(sizeof(char) * (from->size+1));
    if(tmp == NULL)
    {
        return meme_problem;
    }

    for(int i = 0; i < from->size; ++i)
    {
        tmp[i] = (from->arg)[i];
    }
    tmp[from->size] = '\0';

    to->arg = tmp;
    to->size = from->size;

    return well;
}



string* copy_to_new_string(string* from, state* stat)
{
    if(from == NULL ){
        *stat = invalid;
        return NULL;
    }

    string* new = create_string(from->arg, stat);
    if(*stat != well)
    {
        delete_string(new);
        return NULL;
    }
    return new;

}


state cnt_string(string* first, string* second)
{
    if(first == NULL || second == NULL)
    {
        return invalid;
    }
    int new_len = first->size + second->size;
    char* tmp = (char*) malloc(sizeof(char)*(new_len+1));
    if(tmp == NULL)
    {
        return meme_problem;
    }
    for(int i = 0; i < first->size; i++)
    {
        tmp[i] = first->arg[i];
    }
    for(int i = 0; i < second->size; ++i)
    {
        tmp[first->size + i] = second->arg[i];
    }
    tmp[new_len] = '\0';
    free(first->arg);
    first->arg = tmp;
    first->size = new_len;
    return well;

}


