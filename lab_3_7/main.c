#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
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
    last_str,
    none
}state;

typedef enum type{
    delete,
    add,
    modification
}type;

typedef struct stack_node{
    type tp;
    node* pnode;
    node* current_in_list;
}stack_node;

typedef struct undo_stack{
    stack_node* nodes;
    int size;
    int buf;
}undo_stack;

list main_list;
undo_stack stack;

char* read_line(state* stat, FILE* file);
int count_tokens(char *str, const char *delim);
void print_liver(Liver* liver, FILE* file);
input_state parsing_input_data(char* filename, list* lst);
bool is_latin_str(char* str);
state add_node(list* lst, Liver* liver);
node* create_node(Liver* liver);
node* find_same_liver(list* lst, Liver* liver);
void free_list(list* lst);
void print_list(list* lst, FILE* file);
time_t data_compare(char* str1, char* str2);
input_state interactive(list* lst, undo_stack* stack);
Liver* get_liver_stdin(state* stat);
void free_liver(Liver* liver);
void clear_input_buffer(FILE* file);
state add_undo_stack(undo_stack* stack, node* prev, type tp, node* current_in_list);
void delete_stack(undo_stack* stack);
Liver* copy_liver(Liver* searchable);
void print_undo_stack(undo_stack* stack);
void delete_node_list(list* lst, node* tmp);



void signal_handler(int x){//for ctrl+C
    if(x == SIGINT){
        free_list(&main_list);
        delete_stack(&stack);
        exit(0);
    }

}

int main(int argc, char** argv){

    signal(SIGINT, signal_handler);

    if(argc < 2){
        printf("too little arguments!\n");
        return 0;
    }


    stack.size = 0;
    stack.buf = 0;
    stack.nodes = NULL;
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
    input_state status = interactive(&main_list, &stack);
    if(status == is_mem_problem){
        printf("memory problem!\n");
        free_list(&main_list);
        delete_stack(&stack);
        return 0;
    }
    free_list(&main_list);
    delete_stack(&stack);

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
            free(tmp);
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

void free_liver(Liver* liver){
    if(liver == NULL){
        return;
    }
    free(liver->surname);
    liver->surname = NULL;
    free(liver->name);
    liver->name = NULL;
    free(liver->father_name);
    liver->father_name = NULL;
    free(liver->birth_day);
    liver->birth_day = NULL;
    free(liver);
    liver = NULL;

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

void print_list(list* lst, FILE* file){
    if(lst->in_head == NULL){
        printf("the list is empty!\n");
    }

    node* tmp = lst->in_head;
    while(tmp != NULL){
        print_liver(tmp->liver, file);
        tmp = tmp->next;
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

input_state interactive(list* lst, undo_stack* stack){
    int count_of_operation = 0;
    int can_undo = 0;
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

        if (act == 'a') {
            printf("process of adding a new liver has been started\n");
            state stat = well;
            Liver* searchable = get_liver_stdin(&stat);
            if(searchable == NULL && stat == meme_problem){
                return is_mem_problem;
            }
            node* tmp = find_same_liver(lst, searchable);
            if(tmp != NULL && lst->in_head != NULL){
                free_liver(searchable);
                printf("such liver already exist!\n");
                continue;
            }
            stat = add_node(lst, searchable);
            if(stat == meme_problem){
                free_liver(searchable);
                return is_mem_problem;
            }
            if(stat == invalid){
                free_liver(searchable);
                printf("you've entered the NULL-pointer!\n");
                continue;
            }

            node* current_node = find_same_liver(lst, searchable);
            if(current_node == NULL){
                return is_mem_problem;
            }

            Liver* stack_liver = copy_liver(searchable);
            if(stack_liver == NULL){
                return is_mem_problem;
            }

            node* temp = create_node(stack_liver);
            if(temp == NULL){
                free_liver(stack_liver);
                return is_mem_problem;
            }

            Liver* prev = copy_liver(current_node->liver);
            if(prev == NULL){
                free_liver(temp->liver);
                free(temp);

                return is_mem_problem;
            }
            node* current_in_list = create_node(prev);
            if(current_node == NULL){
                free_liver(temp->liver);
                free(temp);
                free_liver(prev);
                return is_mem_problem;
            }

            state stt = add_undo_stack(stack, temp, add, current_in_list);
            if(stt != well){
                free_liver(temp->liver);
                free(temp);
                free_liver(current_in_list->liver);
                free(current_in_list);

                return is_mem_problem;
            }
            print_undo_stack(stack);
            printf("your liver was added\n");
            count_of_operation++;

            continue;
        }
        else if (act == 'c') {
            printf("ok, you want to change some field of somebody, at first we need to find this man:\n");
            state stat = well;
            Liver* searchable = get_liver_stdin(&stat);
            if(searchable == NULL && stat == meme_problem){
                return is_mem_problem;
            }
            node* tmp = find_same_liver(lst, searchable);
            if(tmp == NULL && lst->in_head != NULL){
                free_liver(searchable);
                printf("there no such liver!\n");
                continue;
            }
            while(1){
                printf("what do you wish to change in this liver?\n");
                printf("-------------------------------\n");
                printf("s - surname\n");
                printf("n - name\n");
                printf("p - patronymic\n");
                printf("d - birth data\n");
                printf("m - male\n");
                printf("a - average income\n");
                printf("q - cancel\n-?:\n");
                char choice;
                fflush(stdin);
                //scanf("%c", &act);
                if(scanf("%c", &choice) == EOF){
                    break;
                }
                int ch;
                if ((ch = getchar()) != '\n' && ch != EOF) {
                    clear_input_buffer(stdin);
                    printf("you've inputted wrong action, rerun\n");
                    continue;
                }
                if(choice == 'q'){
                    free_liver(searchable);
                    printf("cancel of operation\n");
                    break;
                }
                else if(choice == 's'){
                    state st = none;
                    char* surname = NULL;
                    while(st != well){
                        printf("\ninput the new surname of liver:\n");
                        surname = read_line(&st, stdin);
                        if(st == meme_problem){
                            free_liver(searchable);
                            return is_mem_problem;
                        }
                        if(st == empty_str){
                            free(surname);
                            printf("surname isn't empty string!\n");
                            continue;
                        }
                        if(!is_latin_str(surname)){
                            free(surname);
                            printf("surname must be a latin string!\n");
                            st = invalid;
                            continue;
                        }
                    }

                    Liver* stack_liver = copy_liver(searchable);
                    if(stack_liver == NULL){
                        free_liver(searchable);
                        free(surname);
                        return is_mem_problem;
                    }
                    free_liver(searchable);
                    free(tmp->liver->surname);
                    tmp->liver->surname = surname;
                    node* tmp2 = create_node(stack_liver);
                    if(tmp2 == NULL){
                        free_liver(stack_liver);
                        return is_mem_problem;
                    }

                    Liver* prev_liver = copy_liver(tmp->liver);
                    if(prev_liver == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }
                    node* tmp3 = create_node(prev_liver);
                    if(tmp3 == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free(prev_liver);
                        return is_mem_problem;
                    }
                    state stt = add_undo_stack(stack, tmp2, modification, tmp3);
                    if(stt != well){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(tmp3->liver);
                        free(tmp3);
                        return is_mem_problem;
                    }
                    printf("the surname was changed!\n");
                    count_of_operation++;
                    print_undo_stack(stack);
                    break;
                }
                else if(choice == 'n'){
                    state st = none;
                    char* name = NULL;
                    while(st != well){
                        printf("\ninput the new name of liver:\n");
                        name = read_line(&st, stdin);
                        if(st == meme_problem){
                            free(searchable);
                            return is_mem_problem;
                        }
                        if(st == empty_str){
                            free(name);
                            printf("name isn't empty string!\n");
                            continue;
                        }
                        if(!is_latin_str(name)){
                            free(name);
                            printf("name must be a latin string!\n");
                            st = invalid;
                            continue;
                        }
                    }

                    Liver* stack_liver = copy_liver(searchable);
                    if(stack_liver == NULL){
                        free(searchable);
                        free(name);
                        return is_mem_problem;
                    }
                    free_liver(searchable);
                    free(tmp->liver->name);
                    tmp->liver->name = name;

                    node* tmp2 = create_node(stack_liver);
                    if(tmp2 == NULL){
                        free_liver(stack_liver);
                        return is_mem_problem;
                    }


                    Liver* prev_liver = copy_liver(tmp->liver);
                    if(prev_liver == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }
                    node* tmp3 = create_node(prev_liver);
                    if(tmp3 == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(prev_liver);
                        return is_mem_problem;
                    }
                    state stt = add_undo_stack(stack, tmp2, modification, tmp3);
                    if(stt != well){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(tmp3->liver);
                        free(tmp3);
                        return is_mem_problem;
                    }
                    printf("the name was changed!\n");
                    count_of_operation++;
                    print_undo_stack(stack);
                    break;

                }
                else if(choice == 'p'){
                    state st = none;
                    char* father_name = NULL;
                    while(st != well && st != empty_str){
                        printf("\ninput the new patronymic of liver:\n");
                        father_name = read_line(&st, stdin);
                        if(st == meme_problem){
                            free_liver(searchable);
                            return is_mem_problem;
                        }
                        if(st == empty_str){
                            free(father_name);
                            father_name = NULL;
                            printf("your patronymic is empty, well!\n");
                            continue;
                        }
                        if(!is_latin_str(father_name)){
                            free(father_name);
                            printf("patronymic must be a latin string!\n");
                            st = invalid;
                            continue;
                        }
                    }
                    Liver* stack_liver = copy_liver(searchable);
                    if(stack_liver == NULL){
                        free_liver(searchable);
                        free(father_name);
                        return is_mem_problem;
                    }
                    free_liver(searchable);
                    free(tmp->liver->father_name);
                    tmp->liver->father_name = father_name;
                    node* tmp2 = create_node(stack_liver);
                    if(tmp2 == NULL){
                        free_liver(stack_liver);
                        return is_mem_problem;
                    }


                    Liver* prev_liver = copy_liver(tmp->liver);
                    if(prev_liver == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }
                    node* tmp3 = create_node(prev_liver);
                    if(tmp3 == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(prev_liver);
                        return is_mem_problem;
                    }
                    state stt = add_undo_stack(stack, tmp2, modification, tmp3);
                    if(stt != well){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free(tmp3->liver);
                        free(tmp3);
                        return is_mem_problem;
                    }
                    printf("the patronymic was changed!\n");
                    count_of_operation++;
                    print_undo_stack(stack);
                    break;

                }
                else if(choice == 'd'){
                    state st = none;
                    char* birth_day = NULL;
                    while(st != well){
                        printf("\ninput the new birth day of liver in format dd.mm.yyyy :\n");
                        birth_day = read_line(&st, stdin);
                        if(st == meme_problem){
                            free_liver(searchable);
                            return is_mem_problem;
                        }
                        if(st == empty_str){
                            free(birth_day);
                            printf("birth day cannot be an empty string!\n");
                            continue;
                        }
                        struct tm in;
                        if(strptime(birth_day, "%d.%m.%Y", &in) == NULL || in.tm_year > 123 || in.tm_year < 10){
                            free(birth_day);
                            printf("incorrect data!\n");
                            st = invalid;
                            continue;
                        }
                    }
                    Liver* stack_liver = copy_liver(searchable);
                    if(stack_liver == NULL){
                        free_liver(searchable);
                        free(birth_day);
                        return is_mem_problem;
                    }

                    node* tmp2 = create_node(stack_liver);
                    if(tmp2 == NULL){
                        free_liver(stack_liver);
                        return is_mem_problem;
                    }

                    free(searchable->birth_day);
                    searchable->birth_day = birth_day;

                    delete_node_list(lst, tmp);

                    state sttt = add_node(lst, searchable);
                    if(sttt != well){
                        free_liver(searchable);
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }

                    Liver* prev_liver = copy_liver(searchable);
                    if(prev_liver == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }
                    node* tmp3 = create_node(prev_liver);
                    if(tmp3 == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(prev_liver);
                        return is_mem_problem;
                    }
                    state stt = add_undo_stack(stack, tmp2, modification, tmp3);
                    if(stt != well){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(tmp3->liver);
                        free(tmp3);
                        return is_mem_problem;
                    }
                    printf("the date of birth was changed!\n");
                    count_of_operation++;
                    print_undo_stack(stack);
                    break;

                }
                else if(choice == 'm'){
                    Liver* stack_liver = copy_liver(searchable);
                    if(stack_liver == NULL){
                        free_liver(searchable);
                        return is_mem_problem;
                    }
                    free_liver(searchable);

                    if(tmp->liver->male == 'M'){
                        tmp->liver->male = 'W';
                    }else{
                        tmp->liver->male = 'M';
                    }

                    node* tmp2 = create_node(stack_liver);
                    if(tmp2 == NULL){
                        free_liver(stack_liver);
                        return is_mem_problem;
                    }


                    Liver* prev_liver = copy_liver(tmp->liver);
                    if(prev_liver == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }
                    node* tmp3 = create_node(prev_liver);
                    if(tmp3 == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(prev_liver);
                        return is_mem_problem;
                    }
                    state stt = add_undo_stack(stack, tmp2, modification, tmp3);
                    if(stt != well){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(tmp3->liver);
                        free(tmp3);
                        return is_mem_problem;
                    }
                    printf("the male was changed to reverse\n");
                    print_undo_stack(stack);
                    count_of_operation++;
                    break;

                }
                else if(choice == 'a'){
                    state st = none;
                    long double amount_income = 0L;
                    while(st != well){
                        printf("\ninput the new income of liver:\n");
                        char* income = read_line(&st, stdin);
                        if(st == meme_problem){
                            free_liver(searchable);
                            return is_mem_problem;
                        }
                        if(st == empty_str){
                            free(income);
                            printf("income cannot be an empty string!\n");
                            continue;
                        }
                        if(strlen(income) > 20){
                            printf("too large/little number!\n");
                            free(income);
                            st = invalid;
                            continue;
                        }
                        char *end;
                        long double average_income = strtold(income, &end);
                        if (*end != '\0' && *end != '\r' && *end != '\n' || end == income || (average_income == 0.0 && errno == ERANGE) || average_income >= HUGE_VAL-10 || average_income <= -HUGE_VAL + 10 || average_income < 0 || income[0] == '-') {
                            printf("you've inputted invalid or negative income!\n");
                            free(income);
                            st = invalid;
                            continue;
                        }
                        free(income);
                        amount_income = average_income;
                    }

                    Liver* stack_liver = copy_liver(searchable);
                    if(stack_liver == NULL){
                        return is_mem_problem;
                    }
                    free_liver(searchable);
                    tmp->liver->average_income = amount_income;
                    node* tmp2 = create_node(stack_liver);
                    if(tmp2 == NULL){
                        free_liver(stack_liver);
                        return is_mem_problem;
                    }

                    Liver* prev_liver = copy_liver(tmp->liver);
                    if(prev_liver == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        return is_mem_problem;
                    }
                    node* tmp3 = create_node(prev_liver);
                    if(tmp3 == NULL){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(prev_liver);
                        return is_mem_problem;
                    }
                    state stt = add_undo_stack(stack, tmp2, modification, tmp3);
                    if(stt != well){
                        free_liver(tmp2->liver);
                        free(tmp2);
                        free_liver(tmp3->liver);
                        free(tmp3);
                        return is_mem_problem;
                    }
                    printf("the average income was changed!\n");
                    count_of_operation++;
                    print_undo_stack(stack);
                    break;

                }
                else{
                    printf("you've inputted wrong action, rerun\n");
                    free_liver(searchable);
                    continue;
                }
            }

            continue;
        }
        else if (act == 'd') {
            if(lst->in_head == NULL){
                printf("the list is empty!\n");
                continue;
            }
            printf("process of deleting a liver has been started\n");
            state stat = well;
            Liver* searchable = get_liver_stdin(&stat);
            if(searchable == NULL && stat == meme_problem){
                return is_mem_problem;
            }
            node* tmp = find_same_liver(lst, searchable);
            if(tmp == NULL && lst->in_head != NULL){
                free_liver(searchable);
                printf("there no such liver!\n");
                continue;
            }

            Liver* stack_liver = copy_liver(searchable);
            if(stack_liver == NULL){
                return is_mem_problem;
            }
            free_liver(searchable);

            node* tmp2 = create_node(stack_liver);
            if(tmp2 == NULL){
                free_liver(stack_liver);
                return is_mem_problem;
            }
            state stt = add_undo_stack(stack, tmp2, delete, NULL);//так как удалил
            if(stt != well){
                free_liver(tmp2->liver);
                free(tmp2);
                return is_mem_problem;
            }

            print_undo_stack(stack);


            delete_node_list(lst, tmp);
            printf("your liver was deleted/\n");
            count_of_operation++;
            continue;
        }
        else if (act == 'p') {
            printf("ok, print to file.\nInput the filename:");
            state stat = none;
            char* filename = NULL;
            while(stat != well){
                filename = read_line(&stat, stdin);
                if(stat == meme_problem){
                    return is_mem_problem;
                }
                if(stat == empty_str){
                    free(filename);
                    printf("filename isn't empty string!\n");
                    continue;
                }

            }
            FILE* file = fopen(filename, "w");
            if(file == NULL){
                printf("something wrong with opening the file: %s\n", filename);
                free(filename);
                stat = invalid;
                continue;
            }
            print_list(lst, file);
            fclose(file);
            free(filename);
            printf("data were wrote, well\n");
            continue;
        }
        else if (act == 'f') {
            printf("procedure of searching liver has been started:\n");
            state stat = well;
            Liver* searchable = get_liver_stdin(&stat);
            if(searchable == NULL && stat == meme_problem){
                return is_mem_problem;
            }

            node* tmp = find_same_liver(lst, searchable);
            if(tmp == NULL){
                printf("there no such liver!\n");
            }else{
                printf("such liver is exist!\n\n");
                print_liver(tmp->liver, stdout);
            }
            free_liver(searchable);

            continue;
        }
        else if (act == 'u') {
            printf("u\n");
            printf("amount elements in stack: %d\n", stack->size);
            printf("count of modification:%d:\n", count_of_operation);
            if(count_of_operation < 2){
                printf("too little operation were done!\n");
                continue;
            }
            if(can_undo > count_of_operation/2){}
            else{
                can_undo = count_of_operation/2;
                can_undo = stack->size - can_undo;
            }

            if(stack->size != can_undo){
                if(stack->nodes[stack->size - 1].tp == add){
                    node* tmp = find_same_liver(lst, stack->nodes[stack->size - 1].current_in_list->liver);
                    if(tmp == NULL){
                        //error
                    }
                    delete_node_list(lst, tmp);

                    free_liver(stack->nodes[stack->size - 1].pnode->liver);
                    free(stack->nodes[stack->size - 1].pnode);
                    if(stack->nodes[stack->size - 1].current_in_list != NULL) {
                        free_liver(stack->nodes[stack->size - 1].current_in_list->liver);
                        free(stack->nodes[stack->size - 1].current_in_list);
                    }
                    stack->size--;
                    stack->buf = stack->size;
                    stack_node* temp = (stack_node*)realloc(stack->nodes , sizeof(stack_node)*(stack->buf));
                    if(temp == NULL){
                        //error
                    }else{
                        stack->nodes = temp;
                    }
                    count_of_operation--;
                    printf("undo was done!\n");
                }
                if(stack->nodes[stack->size - 1].tp == delete){

                    Liver* new_liver = copy_liver(stack->nodes[stack->size - 1].pnode->liver);
                    add_node(lst, new_liver);

                    free_liver(stack->nodes[stack->size - 1].pnode->liver);
                    free(stack->nodes[stack->size - 1].pnode);
                    if(stack->nodes[stack->size - 1].current_in_list != NULL) {
                        free_liver(stack->nodes[stack->size - 1].current_in_list->liver);
                        free(stack->nodes[stack->size - 1].current_in_list);
                    }
                    stack->size--;
                    stack->buf = stack->size;
                    stack_node* temp = (stack_node*)realloc(stack->nodes , sizeof(stack_node)*(stack->buf));
                    if(temp == NULL){
                        //error
                    }else{
                        stack->nodes = temp;
                    }
                    count_of_operation--;
                    printf("undo was done!\n");
                }

                if(stack->nodes[stack->size - 1].tp == modification){
                    node* tmp = find_same_liver(lst, stack->nodes[stack->size - 1].current_in_list->liver);
                    if(tmp == NULL){
                        //error
                    }
                    delete_node_list(lst, tmp);
                    Liver* new_liver = copy_liver(stack->nodes[stack->size - 1].pnode->liver);
                    add_node(lst, new_liver);

                    free_liver(stack->nodes[stack->size - 1].pnode->liver);
                    free(stack->nodes[stack->size - 1].pnode);
                    if(stack->nodes[stack->size - 1].current_in_list != NULL) {
                        free_liver(stack->nodes[stack->size - 1].current_in_list->liver);
                        free(stack->nodes[stack->size - 1].current_in_list);
                    }
                    stack->size--;
                    stack->buf = stack->size;
                    stack_node* temp = (stack_node*)realloc(stack->nodes , sizeof(stack_node)*(stack->buf));
                    if(temp == NULL){
                        //error
                    }else{
                        stack->nodes = temp;
                    }
                    count_of_operation--;
                    printf("undo was done!\n");
                }
            }

            print_undo_stack(stack);

            continue;
        }
        else if (act == 'i') {
            print_list(lst, stdout);
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
    return is_well;
}


Liver* get_liver_stdin(state* stat){
    state st = none;
    char* surname = NULL;
    char* name = NULL;
    char* father_name = NULL;
    char* birth_day = NULL;
    char male;
    long double amount_income;
    while(st != well){
        printf("\ninput the surname of new liver:\n");
        surname = read_line(&st, stdin);
        if(st == meme_problem){
            *stat = meme_problem;
            return NULL;
        }
        if(st == empty_str){
            free(surname);
            printf("surname isn't empty string!\n");
            continue;
        }
        if(!is_latin_str(surname)){
            free(surname);
            printf("surname must be a latin string!\n");
            st = invalid;
            continue;
        }
    }
    printf("surname accept!\n");
    st = none;
    while(st != well){
        printf("\ninput the name of new liver:\n");
        name = read_line(&st, stdin);
        if(st == meme_problem){
            *stat = meme_problem;
            free(surname);
            return NULL;
        }
        if(st == empty_str){
            free(name);
            printf("name isn't empty string!\n");
            continue;
        }
        if(!is_latin_str(name)){
            free(name);
            printf("name must be a latin string!\n");
            st = invalid;
            continue;
        }
    }
    st = none;
    printf("name accept!\n");
    while(st != well && st != empty_str){
        printf("\ninput the patronymic of new liver:\n");
        father_name = read_line(&st, stdin);
        if(st == meme_problem){
            *stat = meme_problem;
            free(name);
            free(surname);
            return NULL;
        }
        if(st == empty_str){
            free(father_name);
            father_name = NULL;
            printf("your patronymic is empty, well!\n");
            continue;
        }
        if(!is_latin_str(father_name)){
            free(father_name);
            printf("patronymic must be a latin string!\n");
            st = invalid;
            continue;
        }
    }
    st = none;
    printf("patronymic accept!\n");
    while(st != well){
        printf("\ninput the birth day of new liver in format dd.mm.yyyy :\n");
        birth_day = read_line(&st, stdin);
        if(st == meme_problem){
            *stat = meme_problem;
            free(name);
            free(surname);
            free(father_name);
            return NULL;
        }
        if(st == empty_str){
            free(birth_day);
            printf("birth day cannot be an empty string!\n");
            continue;
        }
        struct tm in;
        if(strptime(birth_day, "%d.%m.%Y", &in) == NULL || in.tm_year > 123 || in.tm_year < 10){
            free(birth_day);
            printf("incorrect data!\n");
            st = invalid;
            continue;
        }
    }
    st = none;
    printf("birth day accept!\n");
    while(st != well){
        printf("\ninput the male of new liver (M or W) :\n");
        char choice;
        fflush(stdin);
        if(scanf("%c", &choice) == EOF){
            printf("invalid value!\n");
            st = invalid;
            continue;
        }
        int ch;
        if ((ch = getchar()) != '\n' && ch != EOF) {
            clear_input_buffer(stdin);
            printf("you've inputted wrong male, rerun\n");
            st = invalid;
            continue;
        }
        if(choice != 'M' && choice != 'W'){
            printf("incorrect male!\n");
            st = invalid;
            continue;
        }
        male = choice;
        st = well;
    }
    st = none;
    printf("male was accepted!\n");
    while(st != well){
        printf("\ninput the income of new liver:\n");
        char* income = read_line(&st, stdin);
        if(st == meme_problem){
            *stat = meme_problem;
            free(name);
            free(surname);
            free(father_name);
            free(birth_day);
            return NULL;
        }
        if(st == empty_str){
            free(income);
            printf("income cannot be an empty string!\n");
            continue;
        }
        if(strlen(income) > 20){
            printf("too large/little number!\n");
            free(income);
            st = invalid;
            continue;
        }
        char *end;
        long double average_income = strtold(income, &end);
        if (*end != '\0' && *end != '\r' && *end != '\n' || end == income || (average_income == 0.0 && errno == ERANGE) || average_income >= HUGE_VAL-10 || average_income <= -HUGE_VAL + 10 || average_income < 0 || income[0] == '-') {
            printf("you've inputted invalid or negative income!\n");
            free(income);
            st = invalid;
            continue;
        }
        free(income);
        amount_income = average_income;
    }

    Liver* current_liver = (Liver*)malloc(sizeof(Liver));
    if(current_liver == NULL){
        free(surname);
        free(name);
        free(father_name);
        free(birth_day);
        *stat = meme_problem;
        return NULL;
    }
    current_liver->surname =  surname;
    current_liver->name = name;
    current_liver->father_name = father_name;
    current_liver->birth_day = birth_day;
    current_liver->male = male;
    current_liver->average_income = amount_income;
    return current_liver;

}

state add_undo_stack(undo_stack* stack, node* prev, type tp, node* current_in_list)
{
    if(stack == NULL || prev == NULL){
        return invalid;
    }

    if(stack->nodes == NULL){
        stack->buf = 2;
        stack->size = 0;
        stack->nodes = (stack_node*) malloc(sizeof(stack_node)*(stack->buf));
        if(stack->nodes == NULL){
            return meme_problem;
        }
    }

    stack->size++;
    if(stack->size >= stack->buf){
        stack->buf *= 2;
        stack_node* tmp = (stack_node*) realloc(stack->nodes, sizeof(stack_node)*(stack->buf));
        if(tmp == NULL){
            return meme_problem;
        }else{
            stack->nodes = tmp;
        }
    }

    stack->nodes[stack->size - 1].pnode = prev;
    stack->nodes[stack->size - 1].tp = tp;
    stack->nodes[stack->size-1].current_in_list = current_in_list;
    return well;

}

void delete_stack(undo_stack* stack){
    if(stack == NULL){
        return;
    }
    for(int i = 0; i < stack->size; ++i){
        free_liver(stack->nodes[i].pnode->liver);
        free(stack->nodes[i].pnode);
        if(stack->nodes[i].current_in_list != NULL) {
            free_liver(stack->nodes[i].current_in_list->liver);
            free(stack->nodes[i].current_in_list);
        }

    }
    free(stack->nodes);
}

Liver* copy_liver(Liver* searchable){
    if(searchable == NULL){
        return NULL;
    }
    Liver* stack_liver = (Liver*) malloc(sizeof(Liver));
    if(stack_liver == NULL){
        return NULL;
    }
    stack_liver->surname = (char*) malloc(sizeof(char)*(strlen(searchable->surname) + 1));
    if(stack_liver->surname == NULL){
        free(stack_liver);
        return NULL;
    }
    strcpy(stack_liver->surname, searchable->surname);
    stack_liver->name = (char*) malloc(sizeof(char)*(strlen(searchable->name) + 1));
    if(stack_liver->name == NULL){
        free(stack_liver->surname);
        free(stack_liver);
        return NULL;
    }
    strcpy(stack_liver->name, searchable->name);
    stack_liver->father_name = NULL;
    if(searchable->father_name != NULL){
        stack_liver->father_name = (char*) malloc(sizeof(char)*(strlen(searchable->father_name) + 1));
        if(stack_liver->father_name == NULL){
            free(stack_liver->surname);
            free(stack_liver->name);
            free(stack_liver);
            return NULL;
        }
        strcpy(stack_liver->father_name, searchable->father_name);
    }
    stack_liver->birth_day = (char*) malloc(sizeof(char)*(strlen(searchable->birth_day) + 1));
    if(stack_liver->birth_day == NULL){
        free(stack_liver->surname);
        free(stack_liver->name);
        free(stack_liver->father_name);
        free(stack_liver);
        return NULL;
    }
    strcpy(stack_liver->birth_day, searchable->birth_day);
    stack_liver->male = searchable->male;
    stack_liver->average_income = searchable->average_income;

    return stack_liver;
}

void print_undo_stack(undo_stack* stack) {
    if (stack == NULL) {
        return;
    }

    for (int i = 0; i < stack->size; i++) {
        printf("Undo Stack Node %d:\n", i+1);
        printf("Type: %d\n", stack->nodes[i].tp);
        print_liver(stack->nodes[i].pnode->liver, stdout);
        printf("current node in list:\n");
        if(stack->nodes[i].current_in_list == NULL){
            printf("there no current liver as was deleted\n");
        }else{
            print_liver(stack->nodes[i].current_in_list->liver, stdout);
        }

        printf("\n");
    }
}

void delete_node_list(list* lst, node* tmp)
{
    if(lst->in_head == NULL){
        return;
    }
    node* temp = lst->in_head;
    if(temp == tmp){
        lst->in_head = lst->in_head->next;
        free_liver(tmp->liver);
        free(tmp);
        //printf("your liver was deleted/\n");
    }else {
        while (temp->next != tmp) {
            temp = temp->next;
        }
        temp->next = tmp->next;
        free_liver(tmp->liver);
        free(tmp);
        //printf("your liver was deleted/\n");
    }
}