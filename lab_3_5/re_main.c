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

    FILE* out_file = NULL;
    if(!(out_file = fopen(argv[2], "w"))){
        printf("problem with opening file %s\n", argv[2]);
        free_students(&list_of_students, &count_students);
        return 0;
    }

    int coincidences = 0;

    int (*comparators[])(const void *, const void *) = {
            compare_by_id,
            compare_by_surname,
            compare_by_name,
            compare_by_group
    };

    char* menu[] = {
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
            "1 - search by id",
            "2 - search by surname",
            "3 - search by name",
            "4 - search by group",
            "5 - sort by id",
            "6 - sort by surname",
            "7 - sort by name",
            "8 - sort by group",
            "9 - students with high average",
            "10 - exit"
    };


    for(int i = 0; i < count_students; ++i)
    {
        print_student(&list_of_students[i], stdout);
    }

    while(1)
    {
        for(int i = 0; i < 11; ++i)
        {
            printf("%s\n", menu[i]);
        }
        printf("input the choice + string if you choose 1-4 and tap enter if you choose 5-9:\n");
        ui choice;
        char str[4];
        fflush(stdin);
        scanf("%3s", str);

        char* endptr;
        choice = strtoul(str, &endptr, 10);
        if (endptr == str || *endptr != '\0') {
            printf("not valid number\n");
            continue;
        }
        if(choice == 10){
            printf("bye\n");
            break;
        }


        state stat = well;
        fflush(stdin);
        char *strok = read_line(&stat, stdin);
        if (stat == meme_problem) {
            printf("meme problem!\n");
            free_students(&list_of_students, &count_students);
            fclose(out_file);
            return 0;
        }
        if (stat == empty_str && choice < 5) {
            free(strok);
            printf("you've entered the empty string!\n");
            continue;
        }


        switch(choice)
        {
            case 1: {
                printf("Entered the id of student:\n");
                ui id;
                char *enddptr;
                id = strtoul(strok, &enddptr, 10);
                if (enddptr == strok || strok[0] == '-' || *enddptr != '\0' || strlen(strok) > 10) {
                    printf("not valid unsigned int!\n");
                    break;
                }

                double average = 0;
                for (int i = 0; i < count_students; ++i) {
                    if (list_of_students[i].id == id) {
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        for (int j = 0; j < 5; ++j) {
                            average += (list_of_students[i].marks)[j] - '0';
                        }
                        average /= 5;
                        fprintf(out_file, "the average score: %f\n", average);
                    }
                    average = 0;

                }
                if (coincidences == 0) {
                    printf("no such id or not unsigned int\n");
                } else {
                    printf("recorded\n");
                }

                coincidences = 0;
                free(strok);
                break;
            }
            case 2: {
                printf("Entered the surname of student:\n");

                for (int i = 0; i < count_students; ++i) {
                    if (strcmp(list_of_students[i].surname, strok) == 0) {
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        fprintf(out_file, "\n");
                    }

                }
                if (coincidences == 0) {
                    printf("no such surname\n");
                } else {
                    printf("recorded\n");
                }
                coincidences = 0;
                free(strok);
                break;
            }
            case 3: {
                printf("Entered the name of student:\n");

                for (int i = 0; i < count_students; ++i) {
                    if (strcmp(list_of_students[i].name, strok) == 0) {
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        fprintf(out_file, "\n");
                    }

                }
                if (coincidences == 0) {
                    printf("no such name\n");
                } else {
                    printf("recorded\n");
                }
                coincidences = 0;
                free(strok);
                break;
            }
            case 4:{
                printf("Entered the group of student:\n");

                for(int i = 0; i < count_students; ++i){
                    if(strcmp(list_of_students[i].group, strok) == 0){
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        fprintf(out_file, "\n");
                    }

                }
                if(coincidences == 0){
                    printf("no such group\n");
                }else{
                    printf("recorded\n");
                }
                coincidences = 0;
                free(strok);
                break;
            }
            case 5:
            case 6:
            case 7:
            case 8:

                printf("sorted by yours wish:\n");
                qsort(list_of_students, count_students, sizeof(Student), comparators[choice - 5]);
                for(int i = 0; i < count_students; ++i)
                {
                    print_student(&list_of_students[i], stdout);
                }
                free(strok);
                break;

            case 9:
                printf("the values are recorded\n");
                print_average_grades(list_of_students, count_students, out_file);
                free(strok);
                break;
            default:
                printf("You've inputted the wrong choice\n");
                free(strok);
                break;
        }

    }

//    for(int i = 0; i < count_students; ++i)
//    {
//        print_student(&list_of_students[i], stdout);
//    }
    fclose(out_file);
    free_students(&list_of_students, &count_students);
}


