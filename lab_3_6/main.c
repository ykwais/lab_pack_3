#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


typedef enum status{
    well,
    meme_problem,
    invalid
}state;


typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_mem_problem,
    ic_empty_file
}input_status;

typedef enum status_arriving{
    start,
    middle,
    finish
}state_bus;

typedef struct{
    char* bus_id;
    char* arrive;
    char* departure;
    double x;
    double y;
    state_bus st;
}Bus;

typedef struct in_node{
    Bus* bus;
    struct in_node* in_next;
}in_node;

typedef struct list{
    in_node* in_head;
}list;

typedef struct out_node{
    list list_in_node;
    struct out_node* out_next;
}out_node;

typedef struct out_list{
    out_node* out_head;
}out_list;

void free_list_in(list* lst)
{
    in_node* tmp;
    while(lst->in_head != NULL){
        tmp = lst->in_head;
        lst->in_head = lst->in_head->in_next;
        free(tmp->bus->bus_id);
        free(tmp->bus->arrive);
        free(tmp->bus->departure);
        free(tmp->bus);

        free(tmp);
    }

}

void free_list_global(out_list* list)
{
    out_node* tmp;
    while (list->out_head != NULL){
        tmp = list->out_head;
        list->out_head = list->out_head->out_next;
        free_list_in(&tmp->list_in_node);
        free(tmp);
    }
}


out_node* find_bus_number(out_list* list, char* number_bus)
{
    out_node* tmp = list->out_head;
    while(tmp != NULL)
    {
        if(!(strcmp(tmp->list_in_node.in_head->bus->bus_id, number_bus))){
            return tmp;
        }
        tmp = tmp->out_next;
    }
    return NULL;
}


in_node* create_in_node(Bus* bus)
{
    if(bus == NULL){
        return NULL;
    }
    in_node* new_node = (in_node*)malloc(sizeof(in_node));
    if(new_node == NULL){
        return NULL;
    }
    new_node->bus = bus;
    new_node->in_next = NULL;
    return new_node;
}

out_node* create_out_node()
{
    out_node* new_node = (out_node*) malloc(sizeof(out_node));
    if(new_node == NULL)
    {
        return NULL;
    }
    new_node->list_in_node.in_head = NULL;
    new_node->out_next = NULL;
    return new_node;
}

state add_in(list* in_list, Bus* bus)
{
    if(in_list == NULL || bus == NULL){
        return invalid;
    }

    in_node* new_node = create_in_node(bus);
    if(new_node == NULL){
        return meme_problem;
    }
    if(in_list->in_head == NULL)
    {
        in_list->in_head = new_node;
        return well;
    }else{
        in_node* tmp = in_list->in_head;
        while (tmp->in_next != NULL){
            tmp = tmp->in_next;
        }
        tmp->in_next = new_node;
        return well;
    }
}

state add_out(out_list* list, Bus* bus){
    state st;
    out_node* exist_node = find_bus_number(list, bus->bus_id);
    if(exist_node == NULL){
        out_node* new_node = create_out_node();
        if( new_node == NULL){
            return meme_problem;
        }
        st = add_in(&(new_node->list_in_node), bus);
        if(st != well){
            free(new_node);
            return st;
        }
        if(list->out_head == NULL){
            list->out_head = new_node;
            return well;
        }else{
            out_node* tmp = list->out_head;
            while(tmp->out_next != NULL){
                tmp = tmp->out_next;
            }
            tmp->out_next = new_node;
            return well;
        }
    }else{
        st = add_in(&(exist_node->list_in_node), bus);
        return st;
    }
}

void print_in_list(list* lst){
    in_node* tmp = lst->in_head;
    while(tmp != NULL){
        printf("number bus: %s\n", tmp->bus->bus_id != NULL ? tmp->bus->bus_id : "NONE");
        printf("x_coordination: %lf\n", tmp->bus->x);
        printf("y_coordination: %lf\n", tmp->bus->y);
        printf("time arrive: %s\n", tmp->bus->arrive != NULL ? tmp->bus->arrive : "NONE");
        printf("time departure: %s\n", tmp->bus->departure != NULL ? tmp->bus->departure : "NONE");
        printf("the state of bus: %s\n", tmp->bus->st == start ? "START" : tmp->bus->st == finish ? "FINISH" : "MIDDLE");
        tmp = tmp->in_next;
    }
}

void print_all(out_list* list)
{
    int count = 0;
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        print_in_list(&(tmp->list_in_node));
        printf("\nfinal %d\n", count + 1);
        count++;
        tmp = tmp->out_next;
    }
}


double read_and_convert_to_doub(state* stat, FILE* file);
void clear_input_buffer(FILE* file);
input_status parsing_input_data(char* filename, out_list* list);
char* read_line(state* stat, FILE* file);


int main(int argc, char** argv) {
    if(argc < 2){
        printf("too few arguments\n");
    }

    out_list main_list;
    main_list.out_head = NULL;


    for(int i = 1; i < argc; ++i){

        parsing_input_data(argv[i], &main_list);

    }

    print_all(&main_list);
    free_list_global(&main_list);
}

input_status parsing_input_data(char* filename, out_list* list) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return ic_problem_open_file;
    }
    int q = fgetc(file);
    if (q == EOF) {
        fclose(file);
        return ic_empty_file;
    } else {
        fseek(file, 0, SEEK_SET);
    }

    double x, y;
    state st = well;
    x = read_and_convert_to_doub(&st, file);
    if (st == invalid) {
        fclose(file);
        return ic_invalid_string;
    }

    y = read_and_convert_to_doub(&st, file);
    if (st == invalid) {
        fclose(file);
        return ic_invalid_string;
    }

    do
    {
        char* str = read_line(&st, file);
        if(st == invalid){
            fclose(file);
            return ic_invalid_string;
        }
        if(st == meme_problem){
            fclose(file);
            return ic_mem_problem;
        }
        char* bus_number = strtok(str, " ");/////
        char* in_data = strtok(NULL, " ");
        char* in_time = strtok(NULL, " ");
        char* out_data = strtok(NULL, " ");
        char* out_time = strtok(NULL, " ");
        char* type = strtok(NULL, " ");

        struct tm in, out;
        strptime(in_data, "%d.%m.%Y", &in);
        strptime(in_time, "%H:%M:%S", &in);
        char buff_in[20];
        strftime(buff_in, 20, "%d.%m.%Y %H:%M:%S", &in);
        strptime(out_data, "%d.%m.%Y", &out);
        strptime(out_time, "%H:%M:%S", &out);
        char buff_out[20];
        strftime(buff_out, 20, "%d.%m.%Y %H:%M:%S", &out);

        state_bus bs_st = !strcmp(type, "s\r") || !strcmp(type, "s") ? start : !strcmp(type, "p\r") || !strcmp(type, "p") ? middle : finish;

        Bus* cur_bus = (Bus*)malloc(sizeof(Bus));
        if(cur_bus == NULL){
            fclose(file);
            free(str);
            return ic_mem_problem;
        }
        cur_bus->st = bs_st;
        cur_bus->x = x;
        cur_bus->y = y;
        cur_bus->arrive = (char*) malloc(sizeof(char)*(strlen(buff_in) + 1));
        if(cur_bus->arrive == NULL)
        {
            fclose(file);
            free(cur_bus);
            free(str);
            return ic_mem_problem;
        }
        strcpy(cur_bus->arrive, buff_in);
        cur_bus->departure = (char*) malloc(sizeof(char)*(strlen(buff_out) + 1));
        if(cur_bus->departure == NULL)
        {
            fclose(file);
            free(cur_bus->arrive);
            free(cur_bus);
            free(str);
            return ic_mem_problem;
        }
        strcpy(cur_bus->departure, buff_out);
        cur_bus->bus_id = (char*) malloc(sizeof(char)*(1+ strlen(bus_number)));
        if(cur_bus->bus_id == NULL){
            fclose(file);
            free(cur_bus->arrive);
            free(cur_bus->departure);
            free(cur_bus);
            free(str);
            return ic_mem_problem;
        }
        strcpy(cur_bus->bus_id, bus_number);

        st = add_out(list, cur_bus);
        if(st == meme_problem){
            fclose(file);
            free(cur_bus->arrive);
            free(cur_bus->departure);
            free(cur_bus);
            free(str);
            return ic_mem_problem;
        }
        if(st == invalid){
            fclose(file);
            free(cur_bus->arrive);
            free(cur_bus->departure);
            free(cur_bus);
            free(str);
            return ic_invalid_string;
        }
        free(str);

    }
    while(st == well);

    fclose(file);
}

void clear_input_buffer(FILE* file) {
    int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF);
}


double read_and_convert_to_doub(state* stat, FILE* file) {
    char line[256];
    if (!fgets(line, sizeof line, file)) {
        *stat = invalid;
        return 0.0;
    }
    if (line[strlen(line) - 1] != '\n') {
        clear_input_buffer(file);
    }

    char *end;
    errno = 0;
    double val = strtod(line, &end);
    if( errno == ERANGE ||end == line || *end != '\r'){///////////
        *stat = invalid;
        return 0.0;
    }

    *stat = well;
    return val;
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
        *stat = invalid;
        free(result);
        return NULL;
    }
    result[size] = '\0';
    *stat = well;
    return result;
}