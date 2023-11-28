#include "helper.h"

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
        if(st == empty_str){
            fclose(file);
            return ic_empty_str;
        }
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
    return ic_well;
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
    if(strcmp(result, "\0") == 0 || strcmp(result, "\r") == 0){
        *stat = empty_str;
        free(result);
        return NULL;
    }
    *stat = well;
    return result;
}

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
    }
    else if(strcmp(in_list->in_head->bus->arrive, bus->arrive) > 0){////
        new_node->in_next = in_list->in_head;
        in_list->in_head = new_node;

        return well;
    }
    else {
        in_node* tmp = in_list->in_head;
        while (tmp->in_next != NULL && strcmp(tmp->in_next->bus->arrive, bus->arrive) < 0){
            tmp = tmp->in_next;
        }
        new_node->in_next = tmp->in_next;
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
        printf("the state of bus: %s\n\n", tmp->bus->st == start ? "START" : tmp->bus->st == finish ? "FINISH" : "MIDDLE");
        tmp = tmp->in_next;
    }
}

check check_coordination_in(list* lst){
    in_node* tmp = lst->in_head;
    while(tmp != NULL && tmp->in_next != NULL){
        in_node* tmp2 = tmp->in_next;
        if(fabs(tmp2->bus->x - tmp->bus->x) < EPS && fabs(tmp2->bus->y - tmp->bus->y) < EPS){
            return cb_invalid;
        }
        tmp = tmp->in_next;
    }
    return cb_well;
}

check check_coordination(out_list* list){
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        switch (check_coordination_in(&(tmp->list_in_node))) {
            case cb_invalid:
                return cb_invalid;
            case cb_well:
                break;
        }
        tmp = tmp -> out_next;
    }
    return cb_well;
}



same_time check_time_bus_in_list(list* lst){
    in_node* tmp = lst->in_head;
    while( tmp != NULL && tmp->in_next != NULL){
        in_node* tmp2 = tmp->in_next;
        int time_arr = strcmp(tmp->bus->arrive, tmp2->bus->arrive);
        int time_depart = strcmp(tmp->bus->departure, tmp2->bus->departure);
        int time_arr_and_dep = strcmp(tmp->bus->arrive, tmp->bus->departure);
        if(time_arr == 0 && time_depart == 0){
            return same_time_all;
        }
        if(time_arr == 0){
            return same_time_arrive;
        }
        if(time_depart == 0){
            return same_time_departure;
        }
        if(time_arr_and_dep == 0 || strcmp(tmp2->bus->arrive, tmp2->bus->departure) == 0){
            return same_time_arriving_and_departure;
        }
        if(time_arr_and_dep > 0){
            return departure_less_arrive;
        }
        tmp = tmp->in_next;
    }
    in_node* temp = lst->in_head;
    while(temp != NULL){
        in_node* temp_2 = temp->in_next;
        while(temp_2 != NULL){
            if(strcmp(temp->bus->departure, temp_2->bus->departure) == 0){
                return same_time_departure;
            }
            temp_2 = temp_2->in_next;
        }
        temp = temp->in_next;
    }

    tmp = lst->in_head;
    while(tmp != NULL){
        temp = tmp->in_next;
        while(temp != NULL){
            if(strcmp(temp->bus->arrive, tmp->bus->departure) <= 0){
                return departure_more_than_arriving;
            }
            temp = temp->in_next;
        }
        tmp = tmp->in_next;
    }

    return well_time;
}

same_time check_time_bus(out_list* list)
{
    out_node* tmp = list->out_head;
    while(tmp != NULL)
    {
        switch (check_time_bus_in_list(&(tmp->list_in_node))) {
            case same_time_arriving_and_departure:
                return same_time_arriving_and_departure;
            case same_time_all:
                return same_time_all;
            case same_time_departure:
                return same_time_departure;
            case same_time_arrive:
                return same_time_arrive;
            case departure_less_arrive:
                return departure_less_arrive;
            case departure_more_than_arriving:
                return departure_more_than_arriving;
            case well_time:
                break;

        }
        tmp = tmp->out_next;
    }
    return well_time;
}


check check_in_list(list* lst){
    int beg;
    int fin = 0;
    if(lst->in_head->bus->st != start){
        return cb_invalid;
    }else{
        beg = 1;
    }

    in_node* tmp = lst->in_head;
    while(tmp->in_next != NULL){

        tmp = tmp->in_next;
        if(tmp->bus->st == finish && beg == 1){
            beg = 0;
            fin = 1;
            continue;
        }
        else if(tmp->bus->st == start && fin == 1){
            fin = 0;
            beg = 1;
            continue;
        }
        else if(tmp->in_next == NULL){
            break;
        }
        else if(tmp->bus->st == middle && beg == 1 && fin == 0){
            continue;
        }else{
            return cb_invalid;
        }
    }
    if(tmp->bus->st != finish){
        return cb_invalid;
    }
    return cb_well;
}

check check_all(out_list* list){
    out_node* tmp = list->out_head;
    while(tmp != NULL){
        switch (check_in_list(&(tmp->list_in_node))) {
            case cb_invalid:
                return cb_invalid;
            case cb_well:
                break;
        }
        tmp = tmp->out_next;
    }
    return cb_well;
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
