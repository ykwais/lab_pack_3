#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include <string.h>




typedef enum status{
    well,
    meme_problem,
    invalid
}state;


typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_wrong_amount_lex,
    ic_mem_problem,
    ic_the_same_id,
    ic_empty_file
}input_status;

typedef enum status_arriving{
    start,
    middle,
    finish
}state_bus;

typedef struct{
    char* bus_id;
    struct tm arrive;
    struct tm departure;
    double x;
    double y;
    state_bus st;
}Bus;

typedef struct node{
    Bus* bus;
    struct node* next;
}node;

typedef struct list{
    node* current;
    struct list* next;
}list;

void print_bus(Bus* bus){
    printf("number bus: %s\n", bus->bus_id != NULL ? bus->bus_id : "NONE");
    printf("x_coordination: %lf\n", bus->x);
    printf("y_coordination: %lf\n", bus->y);
    char buff[21];
    strftime(buff, 21, "%d.%m.%Y %H:%M:%S", &(bus->arrive));
    printf("time arrive: %s\n", buff);
    strftime(buff, 21, "%d.%m.%Y %H:%M:%S", &(bus->departure));
    printf("time departure: %s\n", buff);
    printf("the state of bus: %s\n", bus->st == start ? "START" : bus->st == finish ? "FINISH" : "MIDDLE");
}

node* find_bus_number(list* main_list, char* bus_number)
{

    list* tmp = main_list;
    while(tmp->current != NULL)
    {
        if(strcmp(tmp->current->bus->bus_id, bus_number) == 0){
            return tmp->current;
        }
        tmp = tmp->next;
    }
    return NULL;

}

state add_to_main_list(list* main_list, Bus* current_bus)
{
    if(main_list == NULL || current_bus == NULL){
        return invalid;
    }
    list* tmp = main_list;
    while(tmp->current != NULL){
        tmp = tmp->next;
    }
    tmp->current = (node*)malloc(sizeof(node));
    if(tmp->current == NULL){
        return meme_problem;
    }
    tmp->current->bus = current_bus;
    tmp->current->next = NULL;
    return well;
}

double read_and_convert_to_doub(state* stat, FILE* file);
void clear_input_buffer(FILE* file);
input_status parsing_input_data(char* filename, list* list);
char* read_line(state* stat, FILE* file);


int main(int argc, char** argv) {
    if(argc < 2){
        printf("too few arguments\n");
    }

    list* main_list = (list*)malloc(sizeof(list));
    if(main_list == NULL)
    {
        printf("meme problem\n");
        return 0;
    }

    main_list->current = NULL;
    main_list->next = NULL;


    for(int i = 1; i < argc; ++i){
//            //TODO free списка
        parsing_input_data(argv[i], main_list);
        //TODO parse data

    }
    list* tmp = main_list;
    while(tmp != NULL){
        print_bus(tmp->current->bus);
        tmp = tmp->next;
    }


}

input_status parsing_input_data(char* filename, list* list) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return ic_problem_open_file;
    }
    int q = fgetc(file);
    if (q == EOF) {
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
            //break;
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

//        char buff_in[20];
//        strftime(buff_in, 20, "%d.%m.%Y %H:%M:%S", &in);
//        printf("%s\n", buff_in);

        strptime(out_data, "%d.%m.%Y", &out);
        strptime(out_time, "%H:%M:%S", &out);

//        char buff_out[20];
//        strftime(buff_out, 20, "%d.%m.%Y %H:%M:%S", &out);
//        printf("%s\n", buff_out);

        state_bus bs_st = !strcmp(type, "s") ? start : !strcmp(type, "p") ? middle : finish;

        Bus current_bus;
        current_bus.st = bs_st;
        current_bus.x = x;
        current_bus.y = y;
        current_bus.arrive = in;
        current_bus.departure = out;
        current_bus.bus_id = bus_number;
        node* is_there = find_bus_number(list, current_bus.bus_id);
        if(is_there == NULL){
            st = add_to_main_list(list, &current_bus);
            if(st == meme_problem){
                free(str);
                return ic_mem_problem;
            }
            if(st == invalid){
                free(str);
                return ic_invalid_string;
            }
        }

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

    double val = strtod(line, &end);


    *stat = well;
    return val;
}

char* read_line(state* stat, FILE* file)
{

    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*buf);
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
            char* tmp = (char*) realloc(result, sizeof(char)*buf);
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