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


int main() {
    state st = well;

    string* test = create_string("", &st);
    if(st == well){
        printf("well: %s\n", test->arg);
        printf("size: %d\n",test->size);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        return 0;
    }

    string* test_2 = create_string("235", &st);
    if(st == well){
        printf("well: %s\n", test_2->arg);
        printf("size: %d\n",test_2->size);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }

    int comp = equal(test, test_2, &st);
    if(st == well){
        printf("comp: %d\n", comp);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }

    int comp_2 = order_relation_string(test, test_2, &st);
    if(st == well){
        printf("comp: %d\n", comp_2);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }

    printf("test concat:\n");
    printf("\nfrom: %s\n", test->arg);
    printf("to: %s\n", test_2->arg);

    switch(cnt_string(test, test_2)){
        case invalid:
            printf("invalid input\n");
            delete_string(test);
            delete_string(test_2);
            return 0;
        case meme_problem:
            printf("mememe problem\n");
            delete_string(test);
            delete_string(test_2);
            return 0;
        case well:


            printf("\nfrom: %s\n", test->arg);
            printf("to: %s\n", test_2->arg);
            break;


    }


    switch(copy_to_exist(test, test_2)){
    case invalid:
        printf("invalid input\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    case meme_problem:
        printf("mememe problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    case well:
        printf("\nfrom: %s\n", test->arg);
        printf("to: %s\n", test_2->arg);
        break;


    }

    string* test_3 = copy_to_new_string(test, &st);
    if(st == well){
        printf("test_3: %s\n", test_3->arg);
        printf("size test_3: %d\n", test_3->size);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        delete_string(test_3);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        delete_string(test_3);

        return 0;
    }

    delete_string(test);
    delete_string(test_2);
    delete_string(test_3);
}




string* create_string(const char* str, state* stat)
{
    if(str == NULL || stat == NULL){
        *stat = invalid;
        return NULL;
    }
    string* new = (string*) malloc(sizeof(string)*1000000000000000000);
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

