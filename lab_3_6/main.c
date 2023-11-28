#include "helper.h"


int main(int argc, char** argv) {
    if(argc < 2){
        printf("too few arguments\n");
    }

    out_list main_list;
    main_list.out_head = NULL;

    for(int i = 1; i < argc; ++i){

        switch(parsing_input_data(argv[i], &main_list)){
            case ic_mem_problem:
                printf("meme problem\n");
                free_list_global(&main_list);
                return 0;
            case ic_problem_open_file:
                printf("problem with open file: %s\n", argv[i]);
                free_list_global(&main_list);
                return 0;
            case ic_invalid_string:
                break;
            case ic_empty_file:
                printf("you've inputted the empty file: %s\n", argv[i]);
                free_list_global(&main_list);
                return 0;
            case ic_empty_str:
                printf("there is empty str in file!\n");
                free_list_global(&main_list);
                return 0;
            case ic_well:
                break;
        }

    }
    if(check_all(&main_list) == cb_invalid){
        printf("incorrect data\n");
        free_list_global(&main_list);
        return 0;
    }
    switch(check_time_bus(&main_list)){
        case same_time_arriving_and_departure:
            printf("you've inputted the same arrive and departure time in one bus\n");
            free_list_global(&main_list);
            return 0;
        case same_time_all:
            printf("you've inputted the same time of arriving and departure for one bus\n");
            free_list_global(&main_list);
            return 0;
        case same_time_departure:
            printf("you've inputted the same time of departure\n");
            free_list_global(&main_list);
            return 0;
        case same_time_arrive:
            printf("you've inputted the same time of arriving\n");
            free_list_global(&main_list);
            return 0;
        case departure_less_arrive:
            printf("you've inputted departure time less arriving time for one bus\n");
            free_list_global(&main_list);
            return 0;
        case departure_more_than_arriving:
            printf("you've inputted departure time more or equal than some other arriving time\n");
            free_list_global(&main_list);
            return 0;
        case well_time:
            break;
    }
    switch (check_coordination(&main_list)) {
        case cb_invalid:
            printf("you've entered the same coordinates for two consecutive stops of the same bus\n");
            free_list_global(&main_list);
            return 0;
        case cb_well:
            break;
    }
    //print_all(&main_list);
    interactive(&main_list);
    free_list_global(&main_list);
}




void interactive(out_list* list)
{
    while(1) {
        printf("==================================================================\n");
        printf("Choose action:\n");
        printf("a - the bus with the most number of routes\n");
        printf("b - the bus with the least number of routes\n");
        printf("с - the bus with the longest track length\n");
        printf("d - the bus with the shortest track length\n");
        printf("e - the bus with the longest route\n");
        printf("f - the bus with the shortest route\n");
        printf("g - the bus with the longest stop\n");
        printf("h - the bus with the shortest stop\n");
        printf("w - the bus with the longest downtime\n");
        printf("i - info about all bus by sorted\n");
        printf("q - exit\n");
        printf("?-: ");
        char act;
        fflush(stdin);
        scanf("%c", &act);
        int c;
        if ((c = getchar()) != '\n' && c != EOF) {
            clear_input_buffer(stdin);
            printf("you've inputted wrong action, rerun\n");
            continue;
        }
        state st = well;
        if (act == 'a') {
            char* tmp = bus_id_route(list, &st, "max");
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the most number of routes: %s\n", tmp);
            continue;
        }
        if (act == 'b') {
            char* tmp = bus_id_route(list, &st, "min");
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the least number of routes: %s\n", tmp);
            continue;
        }
        if (act == 'c') {
            double z;
            char* tmp = bus_id_length_track(list, &st, "max", &z);
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the longest track length (%lf): %s\n",z, tmp);
            continue;
        }
        if (act == 'd') {
            double z;
            char* tmp = bus_id_length_track(list, &st, "min", &z);
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the shortest track length (%lf): %s\n",z, tmp);
            continue;
        }
        if (act == 'e') {
            double z;
            char* tmp = max_route_length_bus_id(list, &st, "max", &z);
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the longest route (%lf): %s\n",z, tmp);
            continue;
        }
        if (act == 'f') {
            double z;
            char* tmp = max_route_length_bus_id(list, &st, "min", &z);
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the shortest route (%lf): %s\n",z, tmp);
            continue;
        }
        if (act == 'g') {
            double z;
            char* tmp = diff_arr_dep(list, &st, "max", &z);
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the longest stop (%lf): %s\n",z, tmp);
            continue;
        }
        if (act == 'h') {
            double z;
            char* tmp = diff_arr_dep(list, &st, "min", &z);
            if(st == invalid){
                printf("something wrong!\n");
                break;
            }
            printf("the bus with the shortest stop (%lf): %s\n",z, tmp);
            continue;
        }
        if (act == 'w') {
            continue;
        }
        if (act == 'i') {
            printf("full list of vehicles:\n");
            print_all(list);
            continue;
        }
        if (act == 'q') {
            printf("bye!\n");
            break;
        }
    }
}

int count_route_in(list* lst){
    if(lst == NULL){
        return -1;
    }

    int count = 0;
    in_node* tmp = lst->in_head;
    while(tmp != NULL){
        if(tmp->bus->st == start || tmp -> bus -> st == finish){
            count++;
        }
        tmp = tmp -> in_next;
    }
    if(count % 2 != 0){
        return -1;
    }
    count /= 2;
    return count;
}


char* bus_id_route(out_list* list, state* stat, char* type)
{
    if(list == NULL){
        *stat = invalid;
        return NULL;
    }
    int max = -1;
    int min = INT_MAX;
    out_node* max_route = NULL;
    out_node* min_route = NULL;
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        int current = count_route_in(&(tmp->list_in_node));
        if(current == -1){
            *stat = invalid;
            return NULL;
        }
        if(current >= max){
            max = current;
            max_route = tmp;
        }
        if(current <= min){
            min = current;
            min_route = tmp;
        }
        tmp = tmp -> out_next;
    }

    if(max_route == NULL || min_route == NULL){
        *stat = invalid;
        return NULL;
    }else{
        if(strcmp(type, "max") == 0){
            return max_route->list_in_node.in_head->bus->bus_id;
        }
        if(strcmp(type, "min") == 0){
            return min_route->list_in_node.in_head->bus->bus_id;
        }
    }
}

double track_length_couple(in_node* first, in_node* second)
{
    if(first == NULL || second == NULL){
        return -1;
    }
    return sqrt((first->bus->x - second->bus->x)*(first->bus->x - second->bus->x) + (first->bus->y - second->bus->y)*(first->bus->y - second->bus->y));
}

double track_length_in(list* lst)
{
    if(lst == NULL){
        return -1.0;
    }
    double length = 0l;
    in_node* tmp = lst->in_head;
    while(tmp->in_next != NULL){
        in_node* tmp2 = tmp->in_next;
        double current = track_length_couple(tmp, tmp2);
        if(fabs(current + 1) < EPS){
            return -1l;
        }
        length += current;
        tmp = tmp->in_next;
    }
    return length;
}

char* bus_id_length_track(out_list* list, state* st, char* type, double* len)
{
    if(list == NULL){
        *st = invalid;
        return NULL;
    }
    double max = -1l;
    double min = MAXFLOAT;
    out_node* max_route = NULL;
    out_node* min_route = NULL;
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        double current = track_length_in(&(tmp->list_in_node));
        if(fabs(current + 1) < EPS){
            *st = invalid;
            return NULL;
        }
        if(current > max){
            max = current;
            max_route = tmp;
        }
        if(current < min){
            min = current;
            min_route = tmp;
        }
        tmp = tmp -> out_next;
    }
    if(max_route == NULL || min_route == NULL){
        *st = invalid;
        return NULL;
    }else{
        if(strcmp(type, "max") == 0){
            *len = max;
            return max_route->list_in_node.in_head->bus->bus_id;
        }
        if(strcmp(type, "min") == 0){
            *len = min;
            return min_route->list_in_node.in_head->bus->bus_id;
        }
    }

}

double longest_route(list* lst, char* type){
    if(lst == NULL){
        return -1;
    }
    double max = -1l;
    double min = MAXFLOAT;
    double size_route = 0l;
    in_node* tmp = lst->in_head;
    while(tmp != NULL && tmp->in_next != NULL){
        in_node* tmp2 = tmp->in_next;
        double current = track_length_couple(tmp, tmp2);
        if(current == -1){
            return -1;
        }
        size_route += current;
        if(tmp2->bus->st == finish){
            if(size_route > max){
                max = size_route;
            }
            if(size_route < min){
                min = size_route;
            }
            size_route = 0l;
            tmp = tmp2->in_next;
            continue;
        }
        tmp = tmp->in_next;
    }
    if(strcmp(type, "max") == 0){
        return max;
    }
    if(strcmp(type, "min") == 0){
        return min;
    }

}

char* max_route_length_bus_id(out_list* list, state* st, char* type, double* len){
    if(list == NULL){
        *st = invalid;
        return NULL;
    }
    double max = -1l;
    double min = MAXFLOAT;
    out_node* max_route = NULL;
    out_node* min_route = NULL;
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        double current = longest_route(&(tmp->list_in_node), type);
        if(fabs(current + 1) < EPS){
            *st = invalid;
            return NULL;
        }
        if(current > max){
            max = current;
            max_route = tmp;
        }
        if(current < min){
            min = current;
            min_route = tmp;
        }
        tmp = tmp -> out_next;
    }
    if(max_route == NULL || min_route == NULL){
        *st = invalid;
        return NULL;
    }else{
        if(strcmp(type, "max") == 0){
            *len = max;
            return max_route->list_in_node.in_head->bus->bus_id;
        }
        if(strcmp(type, "min") == 0){
            *len = min;
            return min_route->list_in_node.in_head->bus->bus_id;
        }
    }
}

double diff_arr_dep_in(list* lst, char* type){
    if(lst == NULL){
        return -1;
    }
    double max = -1l;
    double min = MAXFLOAT;
    in_node* tmp = lst->in_head;
    while(tmp != NULL){
        struct tm in, out;
        strptime(tmp->bus->arrive, "%d.%m.%Y %H:%M:%S", &in);
        strptime(tmp->bus->departure, "%d.%m.%Y %H:%M:%S", &out);

        tzset();
        in.tm_isdst = -1;
        out.tm_isdst = -1;
        time_t first = mktime(&in);
        time_t second = mktime(&out);

        double difference = difftime(second, first);

        if(difference > max ){
            max = difference;
        }
        if(difference < min){
            min = difference;
        }
        tmp = tmp->in_next;
    }
    if(strcmp(type, "max") == 0){
        return max;
    }
    if(strcmp(type, "min") == 0){
        return min;
    }

}

char* diff_arr_dep(out_list* list, state* st, char* type, double* len)
{
    if(list == NULL){
        *st = invalid;
        return NULL;
    }
    double max = -1l;
    double min = MAXFLOAT;
    out_node* max_route = NULL;
    out_node* min_route = NULL;
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        double current = diff_arr_dep_in(&(tmp->list_in_node), type);

        if(current > max){
            max = current;
            max_route = tmp;
        }
        if(current < min){
            min = current;
            min_route = tmp;
        }
        tmp = tmp -> out_next;
    }
    if(max_route == NULL || min_route == NULL){
        *st = invalid;
        return NULL;
    }else{
        if(strcmp(type, "max") == 0){
            *len = max;
            return max_route->list_in_node.in_head->bus->bus_id;
        }
        if(strcmp(type, "min") == 0){
            *len = min;
            return min_route->list_in_node.in_head->bus->bus_id;
        }
    }
}
