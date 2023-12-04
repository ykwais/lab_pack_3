#include "helper.h"

//TODO нужно проверить, что файл, в который записываю != тому, откуда считываю     ✓
//TODO unsigned char для оценок ✓
//TODO уникальные id ✓
//TODO проверить все free и fclose ✓
//TODO все через strtoud или такого рода ✓
//TODO обработка пустого файла ✓
//TODO тестинг Тиме ✓
//TODO убрать все дублирования кода


int main(int argc, char** argv) {

    if(argc != 3)
    {
        printf("wrong number of argc!\n");
        return 0;
    }

    if(!strcmp(argv[1], argv[2])){
        printf("You've inputted the same files!\n");
        return 0;
    }

    int coincidences = 0;
    int count_students = 1;
    Student* list_of_students = (Student*)malloc(sizeof(Student)*count_students);
    if(list_of_students == NULL)
    {
        printf("no mem!\n");
        return 0;
    }

    size_lex is_well_lex = norm_lexemma;
    switch(collect_data(argv[1], &list_of_students, &is_well_lex, &count_students)){
        case ic_problem_open_file:
            printf("problem with file %s opening!\n", argv[1]);

            free(list_of_students);
            return 0;

        case ic_empty_file:
            printf("You've inputted the empty file!\n");
            free(list_of_students);
            return 0;

        case ic_invalid_string:
            printf("An incorrect string was entered.\n");

            if(count_students > 1)
            {
                for(int i = 0; i < count_students; ++i)
                {
                    free(list_of_students[i].marks);
                }
            }
            free(list_of_students);
            return 0;

        case ic_wrong_amount_lex:
            printf("The number of tokens in the input file is incorrect\n");

            if(count_students > 1)
            {
                for(int i = 0; i < count_students; ++i)
                {
                    free(list_of_students[i].marks);
                }
            }
            free(list_of_students);
            return 0;

        case ic_mem_problem:
            printf("mem problem\n");

            if(count_students > 1)
            {
                for(int i = 0; i < count_students; ++i)
                {
                    free(list_of_students[i].marks);
                }
            }

            free(list_of_students);
            return 0;

        case ic_the_same_id:
            printf("You've entered the same id!!!\n");

            if(count_students > 1)
            {
                for(int i = 0; i < count_students; ++i)
                {
                    free(list_of_students[i].marks);
                }
            }
            free(list_of_students);
            return 0;

        case ic_well:
            if(is_well_lex == too_long_lexemma){
                printf("The name, surname, or group string was entered too long. They have been reduced to an acceptable number of characters.\n");
            }
            break;

    }

    FILE* out_file = NULL;
    if(!(out_file = fopen(argv[2], "w"))){
        printf("problem with opening file %s\n", argv[2]);
        for(int i = 0; i < count_students; ++i){
            free(list_of_students[i].marks);
        }
        free(list_of_students);
        return 0;
    }

    while(1)
    {
        printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n1 - search by id\n2 - search by surname\n3 - search by name\n4 - search by group\n5 - sort by id\n6 - sort by surname\n7 - sort by name\n8 - sort by group\n9 - students with high average\n10 - exit\n");
        ui choice = 0;
        char str[100];
        fflush(stdin);
        scanf("%99s", str);

        char* endptr;
        choice = strtoul(str, &endptr, 10);
        if (endptr == str) {
            printf("not number\n");
            continue;
        }
        if(choice == 10){
            printf("bye\n");
            break;
        }
        switch(choice){
            case 1:
                printf("Enter the id of student:\n");
                ui id;
                char st[100];
                fflush(stdin);
                scanf("%99s",  st);
                char* enddptr;
                id = strtoul(st, &enddptr, 10);
                if (enddptr == st) {
                    printf("not unsigned int\n");
                    break;
                }

                double average = 0;
                for(int i = 0; i < count_students; ++i){
                    if(list_of_students[i].id == id){
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        for(int j = 0; j < 5; ++j){
                            average += (list_of_students[i].marks)[j];
                        }
                        average /= 5;
                        fprintf(out_file, "the average score: %f\n", average);
                    }
                    average = 0;

                }
                if(coincidences == 0){
                    printf("no such id or not unsigned int\n");
                }else{
                    printf("recorded\n");
                }

                coincidences = 0;
                break;
            case 2:
                printf("Enter the surname of student:\n");
                char last_name[100];
                fflush(stdin);
                scanf("%99s", last_name);

                for(int i = 0; i < count_students; ++i){
                    if(strcmp(list_of_students[i].surname, last_name) == 0){
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        fprintf(out_file, "\n");
                    }

                }
                if(coincidences == 0){
                    printf("no such surname\n");
                }else{
                    printf("recorded\n");
                }
                coincidences = 0;
                break;
            case 3:
                printf("Enter the name of student:\n");
                char name[100];
                fflush(stdin);
                scanf("%99s", name);

                for(int i = 0; i < count_students; ++i){
                    if(strcmp(list_of_students[i].name, name) == 0){
                        coincidences++;
                        print_student(&list_of_students[i], out_file);
                        fprintf(out_file, "\n");
                    }

                }
                if(coincidences == 0){
                    printf("no such name\n");
                }else{
                    printf("recorded\n");
                }
                coincidences = 0;
                break;
            case 4:
                printf("Enter the group of student:\n");
                char group[100];
                fflush(stdin);
                scanf("%99s", group);

                for(int i = 0; i < count_students; ++i){
                    if(strcmp(list_of_students[i].group, group) == 0){
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
                break;
            case 5:
                printf("Sort by id, well:\n");
                qsort(list_of_students, count_students, sizeof(Student), compare_by_id);
                for(int i = 0; i < count_students; ++i){
                    print_student(&list_of_students[i], stdout);
                    printf("\n");
                }

                break;
            case 6:
                printf("Sort by surname, well:\n");
                qsort(list_of_students, count_students, sizeof(Student), compare_by_surname);
                for(int i = 0; i < count_students; ++i){
                    print_student(&list_of_students[i], stdout);
                    printf("\n");
                }
                break;
            case 7:
                printf("Sort by name, well:\n");
                qsort(list_of_students, count_students, sizeof(Student), compare_by_name);
                for(int i = 0; i < count_students; ++i){
                    print_student(&list_of_students[i], stdout);
                    printf("\n");
                }
                break;
            case 8:
                printf("Sort by group, well:\n");
                qsort(list_of_students, count_students, sizeof(Student), compare_by_group);
                for(int i = 0; i < count_students; ++i){
                    print_student(&list_of_students[i], stdout);
                    printf("\n");
                }
                break;
            case 9:
                printf("the values are recorded\n");
                print_average_grades(list_of_students, count_students, out_file);
                break;

            default:
                printf("You've inputted the wrong choice\n");
                break;
        }

    }


    for(int i = 0; i < count_students; ++i){
        free(list_of_students[i].marks);
    }
    free(list_of_students);
    fclose(out_file);
}




