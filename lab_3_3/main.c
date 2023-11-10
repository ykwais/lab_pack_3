#include "helper.h"


int main(int argc, char** argv) {

    char flag_type = '\0';
    switch(analysis(argc, argv, &flag_type)){
        case vs_problem_input_number:
            printf("wrong number of arguments!\n");
            return 0;
        case vs_problem_input:
            printf("You've inputted invalid flag");
            return 0;
        case vs_well:
            break;
    }


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

    if(count_emp < 1){
        printf("there aren't employers in the file!\n");
        free(list_of_emp);
        return 0;
    }


    FILE* out_file = NULL;
    if(!(out_file = fopen(argv[3], "w"))){
        printf("problem with opening file %s\n", argv[3]);

        free(list_of_emp);
    }

    if(flag_type == 'a'){
        qsort(list_of_emp, count_emp, sizeof(Employee), compare_right);
        print_emps(&list_of_emp, &count_emp, out_file);
    }else{
        qsort(list_of_emp, count_emp, sizeof(Employee), compare_left);
        print_emps(&list_of_emp, &count_emp, out_file);
    }

    fclose(out_file);
    free(list_of_emp);
}


