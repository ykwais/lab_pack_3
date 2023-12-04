#include "helper_2.h"


int main(int argc, char** argv)
{

    if (argc != 3) {
        printf("wrong number of argc!\n");
        return 0;
    }

    if (!strcmp(argv[1], argv[2])) {
        printf("You've inputted the same files!\n");
        return 0;
    }

    int count_students = 1;
    Student* list_of_students = (Student*)malloc(sizeof(Student)*count_students);
    if(list_of_students == NULL)
    {
        printf("no mem!\n");
        return 0;
    }

    int buf = 1;
    switch(collect_data(argv[1], &list_of_students, &count_students, &buf)){
        case ic_problem_open_file:
            printf("problem with file %s opening!\n", argv[1]);
            free_students(&list_of_students, &count_students);
            return 0;

        case ic_empty_file:
            printf("You've inputted the empty file!\n");
            free_students(&list_of_students, &count_students);
            return 0;

        case ic_invalid_string:
            printf("An incorrect string was entered.\n");

            free_students(&list_of_students, &count_students);
            return 0;

        case ic_wrong_amount_lex:
            printf("The number of tokens in the input file is incorrect\n");
            free_students(&list_of_students, &count_students);
            return 0;

        case ic_mem_problem:
            printf("mem problem\n");
            free_students(&list_of_students, &count_students);
            return 0;

        case ic_the_same_id:
            printf("You've entered the same id!!!\n");
            free_students(&list_of_students, &count_students);
            return 0;
        case ic_empty_str:
            printf("you've inputted the empty string in file!\n");
            free_students(&list_of_students, &count_students);
            return 0;

        case ic_well:
            break;
    }

    for(int i = 0; i < count_students; ++i)
    {
        print_student(&list_of_students[i], stdout);
    }
    free_students(&list_of_students, &count_students);
}

