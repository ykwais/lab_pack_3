#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
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
    last_str
}state;

typedef enum type{
    delete,
    add,
    modification
}type;

typedef struct stack_node{
    type tp;
    node* pnode;
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
void print_list(list* lst, char* filename);
time_t data_compare(char* str1, char* str2);
void interactive(list* lst, undo_stack* stack);

list main_list;

void signal_handler(int x){//for ctrl+C
    if(x == SIGINT){
        free_list(&main_list);
        exit(0);
    }

}

int main(int argc, char** argv){

    signal(SIGINT, signal_handler);

    if(argc < 2){
        printf("too little arguments!\n");
        return 0;
    }

    undo_stack stack;
    main_list.in_head = NULL;
    switch(parsing_input_data(argv[1], &main_list)){
        case is_exist_liver:
            printf("you've entered the same livers!\n");
            free_list(&main_list);
            return 0;
        case is_mem_problem:
            printf("there are memory problems!\n");
            free_list(&main_list);
            return 0;
        case is_invalid_string:
            printf("you've inputted the wrong string of data!\n");
            free_list(&main_list);
            return 0;
        case is_empty_file:
            printf("you've inputted the empty file!\n");
            free_list(&main_list);
            return 0;
        case is_problem_open_file:
            printf("problem with opening the file: %s\n", argv[1]);
            free_list(&main_list);
            return 0;
        case is_empty_str:
            printf("there is the empty string in file. It should be fixed!\n");
            free_list(&main_list);
            return 0;
        case is_wrong_amount_lex_in_str:
            printf("you've inputted the wrong amount of tokens in string!\n");
            free_list(&main_list);
            return 0;
        case is_problem_with_male:
            printf("you've inputted the wrong male of liver!\n");
            free_list(&main_list);
            return 0;
        case is_problem_with_token:
            printf("problem with token in file!\n");
            free_list(&main_list);
            return 0;
        case is_well:
            printf("data were collected\n");
            break;
    }

    //print_list(&main_list, "stdout");
    interactive(&main_list, &stack);
    free_list(&main_list);

}


input_state parsing_input_data(char* filename, list* lst) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return is_problem_open_file;
    }
    int q = fgetc(file);
    if (q == EOF) {
        fclose(file);
        return is_empty_file;
    } else {
        fseek(file, 0, SEEK_SET);
    }

    state st = well;
    char *one_full_string = NULL;
    while ((one_full_string = read_line(&st, file)) != NULL) {

        char* tmp = (char*)malloc(sizeof(char)*(strlen(one_full_string)+1));
        if(tmp == NULL){
            free(one_full_string);
            fclose(file);
            return is_mem_problem;
        }
        strcpy(tmp, one_full_string);

        int count_token_in_full = count_tokens(tmp, " ");
        if (count_token_in_full < 5 || count_token_in_full > 6) {
            fclose(file);
            free(one_full_string);
            return is_wrong_amount_lex_in_str;
        }
        free(tmp);

        char *surname = strtok(one_full_string, " ");
        char *name = strtok(NULL, " ");
        char *father_name = NULL;
        if (count_token_in_full == 6) {
            father_name = strtok(NULL, " ");
            if (father_name == NULL) {
                fclose(file);
                free(one_full_string);
                return is_problem_with_token;
            }
        }
        char *birthday = strtok(NULL, " ");
        char *male = strtok(NULL, " ");
        char *income = strtok(NULL, " ");

        if (surname == NULL || name == NULL || birthday == NULL || male == NULL || income == NULL) {
            fclose(file);
            free(one_full_string);
            return is_problem_with_token;
        }

        int len_surname = (int) strlen(surname);
        int len_name = (int) strlen(name);
        int len_pa_name = father_name == NULL ? 0 : (int) strlen(father_name);
        int len_birth = (int) strlen(birthday);
        if (strlen(male) > 1) {
            fclose(file);
            free(one_full_string);
            return is_problem_with_male;
        }
        char male_symb = male[0];
        if (male_symb != 'W' && male_symb != 'M') {
            fclose(file);
            free(one_full_string);
            return is_problem_with_male;
        }

        int len_income = (int) strlen(income);
        if(len_income > 20){
            fclose(file);
            free(one_full_string);
            return is_problem_with_token;
        }
        char *mem_surname = (char *) malloc(sizeof(char) * (len_surname + 1));
        char *mem_name = (char *) malloc(sizeof(char) * (len_name + 1));
        char *mem_father_name  = father_name == NULL ? NULL : (char *) malloc(sizeof(char) * (len_pa_name + 1));
        char *mem_birthday = (char *) malloc(sizeof(char) * (len_birth + 1));
        char *mem_income = (char *) malloc(sizeof(char) * (len_income + 1));

        if((father_name != NULL && mem_father_name == NULL)){
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            fclose(file);
            free(one_full_string);
            return is_mem_problem;
        }

        if (mem_surname == NULL || mem_name == NULL || mem_birthday == NULL ||
            mem_income == NULL) {
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(mem_father_name != NULL) {free(mem_father_name);}
            mem_father_name = NULL;
            fclose(file);
            free(one_full_string);
            return is_mem_problem;

        }


        strcpy(mem_surname, surname);
        strcpy(mem_name, name);

        if (father_name != NULL) {
            strcpy(mem_father_name, father_name);
        }
        strcpy(mem_birthday, birthday);
        strcpy(mem_income, income);

        struct tm in;
        if(strptime(mem_birthday, "%d.%m.%Y", &in) == NULL || in.tm_year > 123 || in.tm_year < 10){
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(mem_father_name != NULL) {free(mem_father_name);}
            mem_father_name = NULL;
            fclose(file);
            free(one_full_string);
            return is_invalid_string;
        }

        if(!is_latin_str(mem_surname) || !is_latin_str(mem_name) || (mem_father_name != NULL && !is_latin_str(mem_father_name))){
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(mem_father_name != NULL) {free(mem_father_name);}
            mem_father_name = NULL;
            fclose(file);
            free(one_full_string);
            return is_invalid_string;
        }

        Liver *new_liver = (Liver *) malloc(sizeof(Liver));
        if (new_liver == NULL) {
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(mem_father_name != NULL) {
                free(mem_father_name);
            }
            fclose(file);
            free(one_full_string);
            return is_mem_problem;
        }

        char *end;
        long double average_income = strtold(mem_income, &end);
        if (*end != '\0' && *end != '\r' && *end != '\n' || end == mem_income || (average_income == 0.0 && errno == ERANGE) || average_income >= HUGE_VAL-10 || average_income <= -HUGE_VAL + 10 || average_income < 0 || mem_income[0] == '-') {
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(mem_father_name != NULL) {
                free(mem_father_name);
            }
            fclose(file);
            free(one_full_string);
            free(new_liver);
            return is_invalid_string;
        }

        new_liver->surname = mem_surname;
        new_liver->name = mem_name;
        new_liver->father_name = mem_father_name;
        new_liver->birth_day = mem_birthday;
        new_liver->male = male_symb;
        new_liver->average_income = average_income;

        //print_liver(new_liver);
        node* temp = find_same_liver(lst, new_liver);
        if(temp != NULL && lst->in_head != NULL){
            fclose(file);
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(father_name != NULL && mem_father_name != NULL){
                free(mem_father_name);
            }
            free(one_full_string);
            free(new_liver);
            return is_exist_liver;
        }

        st = add_node(lst, new_liver);
        if(st == meme_problem){
            fclose(file);
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(father_name != NULL && mem_father_name != NULL){
                free(mem_father_name);
            }
            free(one_full_string);
            free(new_liver);
            return is_mem_problem;
        }
        if(st == invalid){
            fclose(file);
            free(mem_surname);
            free(mem_name);
            free(mem_birthday);
            free(mem_income);
            if(father_name != NULL && mem_father_name != NULL){
                free(mem_father_name);
            }
            free(one_full_string);
            free(new_liver);
            return is_invalid_string;
        }

        free(mem_income);
        free(one_full_string);

    }
    if (st == meme_problem) {
        fclose(file);
        return is_mem_problem;
    }

    if(st == empty_str){
        fclose(file);
        return is_empty_str;
    }
    fclose(file);
    return is_well;
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
    if(strcmp(result, "\0") == 0 || strcmp(result, "\r") == 0 || strcmp(result, "\n") == 0){
        *stat = empty_str;
        free(result);
        return NULL;
    }
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

void print_liver(Liver* liver, FILE* file){

    fprintf(file,"surname: %s\n", liver->surname);
    fprintf(file,"name: %s\n", liver->name);
    fprintf(file,"father_name: %s\n", liver->father_name == NULL ? "no" : liver->father_name);
    fprintf(file,"birthday: %s\n", liver->birth_day);
    fprintf(file,"male: %c\n", liver->male);
    fprintf(file,"average income: %Lf\n\n", liver->average_income);

}

bool is_latin_str(char* str)
{
    if(str == NULL){
        return false;
    }
    int len = (int)strlen(str);
    for(int i = 0; i < len; ++i){
        if(!isalpha(str[i])){
            return false;
        }
    }
    return true;

}

node* create_node(Liver* liver){
    if(liver == NULL){return NULL;}
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
        return NULL;
    }
    new_node->liver = liver;
    new_node->next = NULL;
    return new_node;
}



state add_node(list* lst, Liver* liver){
    if(lst == NULL || liver == NULL){
        return invalid;
    }
    node* new_node = create_node(liver);
    if(new_node == NULL){
        return meme_problem;
    }
    if(lst->in_head == NULL){
        lst->in_head = new_node;
        return well;
    }
    else if(data_compare(lst->in_head->liver->birth_day, new_node->liver->birth_day) < 0){
        new_node->next = lst->in_head;
        lst->in_head = new_node;
        return well;
    }
    else{
        node* tmp = lst->in_head;
        while(tmp->next != NULL && data_compare(tmp->next->liver->birth_day, liver->birth_day) > 0 ){
            tmp = tmp->next;
        }
        new_node->next = tmp->next;
        tmp->next = new_node;
        return well;
    }

}

node* find_same_liver(list* lst, Liver* liver){
    if(lst->in_head == NULL){
        return NULL;
    }
    node* tmp = lst->in_head;
    while(tmp != NULL){
        int surname = strcmp(tmp->liver->surname, liver->surname);
        int name = strcmp(tmp->liver->name, liver->name);
        int father_name;
        if(liver->father_name != NULL && tmp->liver->father_name != NULL){
            father_name = strcmp(tmp->liver->father_name, liver->father_name);
        }else{
            father_name = 1;
        }

        int birth = strcmp(tmp->liver->birth_day, liver->birth_day);
        if(!surname && !name && ((!father_name ) || (liver->father_name == NULL && tmp->liver->father_name == NULL)) && !birth && tmp->liver->male == liver->male && fabsl(tmp->liver->average_income - liver->average_income) < EPS){
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}


void free_list(list* lst){
    node* tmp = NULL;
    while(lst->in_head != NULL){
        tmp = lst->in_head;
        lst->in_head = lst->in_head->next;
        free(tmp->liver->surname);
        free(tmp->liver->name);
        free(tmp->liver->father_name);
        free(tmp->liver->birth_day);
        free(tmp->liver);
        free(tmp);
    }

}

void print_list(list* lst, char* filename){
    if(strcmp(filename, "stdout") == 0){
        node* tmp = lst->in_head;
        while(tmp != NULL){
            print_liver(tmp->liver, stdout);
            tmp = tmp->next;
        }
    }else{
        FILE* file = fopen(filename, "w");
        if(file == NULL){
            printf("problem with opening file: %s\n", filename);
            return;
        }
        node* tmp = lst->in_head;
        while(tmp != NULL){
            print_liver(tmp->liver, file);
            tmp = tmp->next;
        }

    }

}

time_t data_compare(char* str1, char* str2)
{
    struct tm first, second;
    time_t t1, t2;

    strptime(str1, "%d.%m.%Y", &first);
    strptime(str2, "%d.%m.%Y", &second);

    tzset();
    first.tm_sec = 0;
    second.tm_sec = 0;
    first.tm_min = 0;
    second.tm_min = 0;
    first.tm_hour = 0;
    second.tm_hour = 0;
    first.tm_isdst = -1;
    second.tm_isdst = -1;

    t1 = mktime(&first);
    t2 = mktime(&second);

    return t1 - t2;

}

void clear_input_buffer(FILE* file) {
    int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF);
}

void interactive(list* lst, undo_stack* stack){
    while(1) {
        printf("==================================================================\n");
        printf("Choose action:\n");
        printf("a - add a new liver\n");
        printf("с - change the data of liver\n");
        printf("d - delete liver\n");
        printf("p - print full data of livers in file\n");
        printf("f - find the liver by your data\n");
        printf("u - undo\n");
        printf("i - info about all livers by sorted\n");
        printf("q - exit\n");
        printf("?-: ");
        char act;
        fflush(stdin);
        //scanf("%c", &act);
        if(scanf("%c", &act) == EOF){
            break;
        }
        int c;
        if ((c = getchar()) != '\n' && c != EOF) {
            clear_input_buffer(stdin);
            printf("you've inputted wrong action, rerun\n");
            continue;
        }
        state st = well;
        if (act == 'a') {
            printf("a\n");
            continue;
        }
        else if (act == 'c') {
            printf("c\n");
            continue;
        }
        else if (act == 'd') {
            printf("d\n");
            continue;
        }
        else if (act == 'p') {
            printf("p\n");
            continue;
        }
        else if (act == 'f') {
            printf("f\n");
            continue;
        }
        else if (act == 'u') {
            printf("u\n");
            continue;
        }
        else if (act == 'i') {
            print_list(lst, "stdout");
            continue;
        }
        else if (act == 'q') {
            printf("bye!\n");
            break;
        }
        else{
            printf("you've inputted wrong action, rerun\n");
            continue;
        }
    }
}
