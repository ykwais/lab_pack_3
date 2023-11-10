#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define MAX_LENGTH_STR 100
#define COUNT_STR 2

typedef unsigned int ui;

typedef struct
{
    ui id;
    char name[MAX_LENGTH_STR];
    char sur_name[MAX_LENGTH_STR];
    double money;
} Employee;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_invalid_string,
    ic_wrong_amount_lex,
    ic_mem_problem

}input_status;

typedef enum lexemma{
    too_long_lexemma,
    norm_lexemma
} size_lex;

input_status collect_data(char* filename, Employee** list, size_lex* len_lex, int* stud_count);



int main(int argc, char** argv) {
    if(argc != 4){
        printf("wrong number of arguments!\n");
        return 0;
    }

    int coincidences = 0;
    int count_emp = 1;
    Employee * list_of_emp = (Employee*)malloc(sizeof(Employee)*count_emp);
    if(list_of_emp == NULL)
    {
        printf("no mem!\n");
        return 0;
    }

    size_lex is_well_lex = norm_lexemma;
    switch(collect_data(argv[1], &list_of_emp, &is_well_lex, &count_emp)){
        case ic_problem_open_file:
            printf("problem with file %s opening!\n", argv[1]);

            free(list_of_emp);
            return 0;
        case ic_invalid_string:
            printf("An incorrect string of the student's first or last name was entered.\n");

            free(list_of_emp);
            return 0;
        case ic_wrong_amount_lex:
            printf("The number of tokens in the input file is incorrect\n");

            free(list_of_emp);
            return 0;
        case ic_mem_problem:
            printf("mem problem");

            free(list_of_emp);
            return 0;
        case ic_well:
            if(is_well_lex == too_long_lexemma){
                printf("The name, surname, or group string was entered too long. They have been reduced to an acceptable number of characters.\n");
            }
            break;

    }

    for(int i = 0; i < count_emp; ++i){
        printf("%u\n", list_of_emp[i].id);
        printf("%s\n", list_of_emp[i].name);
        printf("%s\n", list_of_emp[i].sur_name);
        printf("%f\n", list_of_emp[i].money);
    }

    FILE* out_file = NULL;
    if(!(out_file = fopen(argv[2], "w"))){
        printf("problem with opening file %s\n", argv[2]);

        free(list_of_emp);
    }
}


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
        if (endptr == first_uint || first_uint[0] == '-') {
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
            if (enddptr == first_uint || first_uint[0] == '-') {
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
