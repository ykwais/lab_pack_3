
#ifndef LAB_3_6_HELPER_H
#define LAB_3_6_HELPER_H

#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <signal.h>

#define EPS 1e-5


typedef enum status{
    well,
    meme_problem,
    invalid,
    empty_str
}state;

typedef enum check_bus_time_arrive{
    cb_well,
    cb_invalid
}check;

typedef enum check_equal{
    same_time_arrive,
    same_time_departure,
    same_time_all,
    same_time_arriving_and_departure,
    departure_less_arrive,
    departure_more_than_arriving,
    well_time
}same_time;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_mem_problem,
    ic_empty_file,
    ic_empty_str,
    ic_same_coordination
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




double read_and_convert_to_doub(state* stat, FILE* file);
void clear_input_buffer(FILE* file);
input_status parsing_input_data(char* filename, out_list* list);
char* read_line(state* stat, FILE* file);
void print_all(out_list* list);
void print_in_list(list* lst);
state add_out(out_list* list, Bus* bus);
state add_in(list* in_list, Bus* bus);
out_node* create_out_node();
in_node* create_in_node(Bus* bus);
out_node* find_bus_number(out_list* list, char* number_bus);
void free_list_global(out_list* list);
void free_list_in(list* lst);
check check_all(out_list* list);
check check_in_list(list* lst);
same_time check_time_bus(out_list* list);
same_time check_time_bus_in_list(list* lst);
check check_coordination_in(list* lst);
check check_coordination(out_list* list);
int count_route_in(list* lst);
char* bus_id_route(out_list* list, state* stat, char* type);
void interactive(out_list* list);
double track_length_couple(in_node* first, in_node* second);
double track_length_in(list* lst);
char* bus_id_length_track(out_list* list, state* st, char* type, double* len);
char* max_route_length_bus_id(out_list* list, state* st, char* type, double* len);
double longest_route(list* lst, char* type);
char* diff_arr_dep(out_list* list, state* st, char* type, double* len);
double diff_arr_dep_in(list* lst, char* type);
char* stop_time(out_list* list, state* st, double* len);
double stop_time_in(list* lst);
int check_coordination_file(out_list* list, double x, double y);
int check_coordination_file_in(list* lst, double x, double y);

#endif //LAB_3_6_HELPER_H
