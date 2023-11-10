#include "helper.h"

input_status collect_data(char* filename, Employee** list, size_lex* len_lex, int* stud_count){
    FILE* file = NULL;
    if(!(file = fopen(filename, "r"))) {
        return ic_problem_open_file;
    }

    ui current_id = 0;
    double current_zp = 0;

    char arr_str[COUNT_STR][MAX_LENGTH_STR];
    char first_uint[100];
    char first_double[100];

    int c;
    int counter = 0;
    int counter_of_students = 0;


    while(fscanf(file, "%s", first_uint) == 1){

        char* endptr;
        current_id = strtoul(first_uint, &endptr, 10);
        if (endptr == first_uint || first_uint[0] == '-' || *endptr != '\0') {
            return ic_invalid_string;
        }

        if(current_id <= UINT_MAX-1 && current_id > 0){

        }else{
            return ic_invalid_string;
        }



        for(int i = 0; i<COUNT_STR; ++i){
            if(fscanf(file, "%99s", arr_str[i]) == 1){
                while ((c = fgetc(file)) != EOF && c != ' ' && c != '\n') {
                    counter++;
                }
            }else{
                return ic_wrong_amount_lex;
            }
        }
        if(counter > 0){
            *len_lex = too_long_lexemma;
        }

        for(int i = 0; i < COUNT_STR; ++i){
            int j = 0;
            while(arr_str[i][j] != '\0') {
                if (!isalpha(arr_str[i][j])) {
                    return ic_invalid_string;
                }
                j++;
            }
        }



        if(fscanf(file, "%s", first_double) == 1){
            char* enddptr;
            current_zp = strtod(first_double, &enddptr);
            if (enddptr == first_double || first_double[0] == '-' || *enddptr != '\0') {
                return ic_invalid_string;
            }

        }else{
            return ic_wrong_amount_lex;
        }




        counter_of_students++;
        if(counter_of_students >= *stud_count){
            *stud_count *= 2;
            Employee * tmp = (Employee *)realloc(*list, sizeof(Employee)*(*stud_count));
            if(tmp == NULL){
                return ic_mem_problem;
            }else{
                *list = tmp;
            }
        }

        (*list)[counter_of_students-1].id = current_id;
        strcpy((*list)[counter_of_students-1].name, arr_str[0]);
        strcpy((*list)[counter_of_students-1].sur_name, arr_str[1]);
        (*list)[counter_of_students-1].money = current_zp;

    }

    if(fgetc(file) != EOF){
        return ic_wrong_amount_lex;
    }
    *stud_count = counter_of_students;
    fclose(file);
    return ic_well;
}

validate analysis(int argc, char** argv, char* flag){
    if(argc != 4 || strlen(argv[2]) != 2)
    {
        return vs_problem_input_number;
    }
    if((argv[2][0] != '-' && argv[2][0] != '/') || (argv[2][1] != 'a' && argv[2][1] != 'd')){
        return vs_problem_input;
    }
    *flag = argv[2][1];
    return vs_well;

}

int compare_right(const void *a, const void *b)
{
    Employee *first = (Employee *)a, *second =(Employee *)b;
    int res;

    res = first->money - second->money;
    if (res != 0) {return res; }

    res = strcmp(first->sur_name, second->sur_name);
    if (res != 0) {return res; }

    res = strcmp(first->name, second->name);
    if (res != 0) {return res; }

    res = first->id - second->id;
    return res;
}


int compare_left(const void *a, const void *b)
{
    return compare_right(b, a);
}

void print_emps(Employee** list, int* count, FILE* file)
{
    for(int i = 0; i < *count; ++i){
        fprintf(file,"ID: %u; ", (*list)[i].id);
        fprintf(file,"Name: %s; ", (*list)[i].name);
        fprintf(file,"Surname: %s; ", (*list)[i].sur_name);
        fprintf(file,"Money: %f;\n", (*list)[i].money);
    }
}

