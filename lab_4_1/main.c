#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#define HASHSIZE 128

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
state paste_to_string(char **string, const char *old, const char *new);


int main(int argc, char** argv) {
    if(argc != 3) {
        print_state(wrong_amount_of_arguments);
        return 0;
    }
    if(strcmp(argv[1], argv[2]) == 0){
        print_state(same_file);
        return 0;
    }
    HashTable* ht = create_hashtable(HASHSIZE);
    if(ht == NULL){
        print_state(meme_problem);
        return 0;
    }

    state st = file_work(argv[1], argv[2], &ht);

    remove_hashtable(ht);

    print_state(st);
    return 0;

}


void print_state(state st)
{
    switch(st){
        case well:
            printf("well\n");
            break;
        case meme_problem:
            printf("meme_problem\n");
            break;
        case empty_file:
            printf("empty_file\n");
            break;
        case same_file:
            printf("same_file\n");
            break;
        case wrong_amount_of_arguments:
            printf("wrong_amount_of_arguments\n");
            break;
        case problem_with_file_opening:
            printf("problem_with_file_opening\n");
            break;
        default:
            printf("unknown status\n");
            break;
    }
}

state get_hash(char* key, HashTable* ht, unsigned long long* result)
{
    unsigned long long hash = 0;
    int base = 62;

    int len = (int)strlen(key);
    for(int i = len-1; i >= 0; i--)
    {
        int value;
        if(key[i] >= 'a' && key[i] <= 'z')
        {
            value = key[i] - 'a' + 10;
        }
        else if(key[i] >= 'A' && key[i] <= 'Z')
        {
            value = key[i] - 'A' + 36;
        }
        else if(key[i] >= '0' && key[i] <= '9')
        {
            value = key[i] - '0';
        }
        else{
            return invalid_data;
        }
        if(ULLONG_MAX - hash < value){
            return invalid_data;
        }
        hash = hash * base + value;
    }

    *result = hash % ht->size;

    return well;
}

bool is_correct_define(char* str)
{
    int len = (int)strlen(str);
    for(int i = 0; i < len; i++)
    {
        if(!isdigit(str[i]) && !isalpha(str[i]))
        {
            return false;
        }
    }
    return true;
}

state insert_in_ht(HashTable* ht, char* key, char* value)
{

    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL)
    {
        return meme_problem;
    }

    new_node->key = key;
    new_node->value = value;

    unsigned long long int index;
    state st = get_hash(key, ht, &index);
    if(st != well){
        free(new_node);
        free(key);
        free(value);
        return st;
    }

    new_node->next = ht->table[index];
    ht->table[index] = new_node;

    return well;


}

HashTable* create_hashtable(unsigned int size)
{
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if(ht == NULL)
    {
        return NULL;
    }
    ht->size = size;
    ht->table = (Node**)calloc(size, sizeof(Node*));
    if(ht->table == NULL)
    {
        free(ht);
        return NULL;
    }
    return ht;
}

void remove_hashtable(HashTable* ht)
{
    if(ht != NULL)
    {
        for(int i = 0; i < ht->size; i++){
            Node* tmp = ht->table[i];
            while(tmp != NULL){
                Node* tmp_next = tmp->next;
                free(tmp->key);
                free(tmp->value);
                free(tmp);
                tmp = tmp_next;
            }
        }
        free(ht->table);
        free(ht);
    }
}

state file_work(char* input_name, char* out_name, HashTable** ht)
{
    FILE* input = fopen(input_name, "r");
    if(input == NULL)
    {
        return problem_with_file_opening;
    }
    int q = fgetc(input);
    if (q == EOF) {
        fclose(input);
        return empty_file;
    } else {
        fseek(input, 0, SEEK_SET);
    }

    FILE* out = fopen(out_name, "w");
    if(out == NULL)
    {
        fclose(input);
        return problem_with_file_opening;
    }

    char* line = NULL;
    state st = well;
    char* ptr = NULL;
    while((line = read_line(&st, input)) != NULL)
    {
        ptr = strstr(line, "#define");
        if(ptr != NULL){
            char* define_line = read_define_line(&st, ptr);
            if(st != well)
            {
                fclose(input);
                fclose(out);
                free(line);
                return st;
            }
            char* tmp = (char*)malloc(sizeof(char)*(strlen(define_line)+1));
            if(tmp == NULL)
            {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                return meme_problem;
            }
            strcpy(tmp, define_line);
            int count = count_tokens(tmp, " ");
            if(count != 3)
            {
                fclose(input);
                fclose(out);
                free(line);
                free(tmp);
                free(define_line);
                return invalid_data;
            }
            free(tmp);
            tmp = NULL;

            char* token = strtok(define_line, " ");

            char* def_name = read_define_line(&st, strtok(NULL, " "));
            if(st != well)
            {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                return st;
            }

            char* value = read_define_line(&st, strtok(NULL, " "));
            if(st != well)
            {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                return st;
            }

            if(!is_correct_define(def_name) || !is_correct_define(value))
            {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                free(def_name);
                free(value);
                return invalid_data;
            }

            st = balance_insert(ht, def_name, value);
            if(st != well)
            {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                free(def_name);
                free(value);
                return st;
            }
            //work with def_name && value
            //free(define_line);
            //free(line);

        }
        else{
            for(int i = 0; i < (*ht)->size; i++){
                Node* tmp = (*ht)->table[i];
                while(tmp != NULL)
                {
                    st = paste_to_string(&line, tmp->key, tmp->value);
                    if(st != well){
                        fclose(input);
                        fclose(out);
                        free(line);
                        return st;

                    }
                    tmp = tmp->next;
                }
            }
        }
        fprintf(out, "%s", line);
        if(line != NULL)
        {
            free(line);
        }
    }
    if(st == meme_problem)
    {
        fclose(input);
        fclose(out);
        return st;
    }




    fclose(input);
    fclose(out);
    return well;

}

char* read_define_line(state* st, char* ptr)
{
    int size = 0;
    int buf = 20;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL)
    {
        *st = meme_problem;
        return NULL;
    }
    while(*ptr != '\n' && *ptr != '\0')
    {
        size++;
        if(size >= buf)
        {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL)
            {
                free(result);
                *st = meme_problem;
                return NULL;
            }
            result = tmp;

        }
        result[size-1] = *ptr;

        ptr++;
    }
    result[size] = '\0';
    *st = well;
    return result;

}

char* read_line(state* stat, FILE* file)
{

    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL){
        *stat = meme_problem;
        return NULL;
    }
    int ch;
    while((ch = fgetc(file)) != EOF && ch != '\n'){
        size++;
        if(size >= buf)
        {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL)
            {
                free(result);
                *stat = meme_problem;
                return NULL;
            }else{
                result = tmp;
            }
        }
        result[size-1] = (char)ch;
    }
    if(size == 0 && ch == EOF){
        *stat = last_str;
        free(result);
        return NULL;
    }
    result[size] = '\0';

    *stat = well;
    return result;
}

int count_tokens(char *str, const char *delim) {
    int count = 0;
    char *token = strtok(str, delim);
    while (token != NULL) {
        count++;
        token = strtok(NULL, delim);
    }
    return count;
}

state resize_ht(HashTable** ht, unsigned int new_size) {
    HashTable *tmp = create_hashtable(new_size);
    if (tmp == NULL) {
        return meme_problem;
    }

    for(int i = 0; i < (*ht)->size; i++)
    {
        Node* tmp_node = (*ht)->table[i];
        while(tmp_node != NULL)
        {
            state st = insert_in_ht(tmp, tmp_node->key, tmp_node->value);
            if(st != well){
                remove_hashtable(tmp);
                return st;
            }
            tmp_node = tmp_node->next;
        }
    }
    remove_hashtable(*ht);
    *ht = tmp;
    return well;
}

state balance_insert(HashTable** ht, char* key, char* value)
{
    state st = insert_in_ht(*ht, key, value);
    if(st != well)
    {
        return st;
    }

    int min_len_list = INT_MAX;
    int max_len_list = 0;

    for(int i = 0; i < (*ht)->size; i++)
    {
        Node* tmp = (*ht)->table[i];
        int len_list = 0;
        while(tmp != NULL){
            len_list++;
            tmp = tmp->next;
        }
        if(len_list > max_len_list)
        {
            max_len_list = len_list;
        }
        if(len_list < min_len_list)
        {
            min_len_list = len_list;
        }
    }

    if(max_len_list >= 2*min_len_list)
    {
        st = resize_ht(ht, 2*(*ht)->size);
        if(st != well){
            return st;
        }
    }
    return well;
}

bool in_alpthabet(const char symb)
{
    return (isalpha(symb) || isdigit(symb));
}

state paste_to_string(char **string, const char *old, const char *new)
{
    char *str = *string;
    int lent = (int)strlen(str), lenn = (int)strlen(old), delta = -lenn + (int)strlen(new);
    for (int i = 0; i < (lent - lenn); i++)
    {
        if ((strncmp(str + i, old, lenn) == 0) && (i == 0 || !in_alpthabet(*(str + i - 1))) && (i > lent - 1 || !in_alpthabet(*(str + i + 1))))
        {
            lent += delta;
            if (delta > 0)
            {
                char *temp = (char *)realloc(str, sizeof(char) * (lent + 1));
                if (temp == NULL)
                {
                    free(str);
                    return meme_problem;
                }
                str = temp;
            }

            memmove(str + i + strlen(new), str + i + lenn, strlen(str + i + lenn) + 1);
            memcpy(str + i, new, strlen(new));
            i += lenn - 1;
        }
    }
    *string = str;
    return well;
}