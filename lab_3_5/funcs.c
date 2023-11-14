#include "helper.h"


input_status collect_data(char* filename, Student** list, size_lex* len_lex, int* stud_count){
    FILE* file = NULL;
    if(!(file = fopen(filename, "r"))) {
        return ic_problem_open_file;
    }

    ui current_id = 0;

    char arr_str[COUNT_STR][MAX_LENGTH_STR];
    char name_group[50];
    char first_uint[100];
    unsigned char first;
    unsigned char arr_mark[5];
    int c;
    int counter = 0;
    int counter_of_students = 0;


    while(fscanf(file, "%s", first_uint) == 1){

        char* endptr;
        current_id = strtoul(first_uint, &endptr, 10);
        if (endptr == first_uint || first_uint[0] == '-' || *endptr != '\0') {
            fclose(file);
            return ic_invalid_string;
        }

        if(current_id <= UINT_MAX-1 && current_id > 0){
            for(int i = 0; i < counter_of_students; ++i)
            {
                if(current_id == (*list)[i].id){
                    fclose(file);
                    return ic_the_same_id;
                }
            }

        }else{
            fclose(file);
            return ic_invalid_string;
        }



        for(int i = 0; i<COUNT_STR; ++i){
            if(fscanf(file, "%99s", arr_str[i]) == 1){
                while ((c = fgetc(file)) != EOF && c != ' ' && c != '\n' && c != '\t') {
                    counter++;
                }
            }else{
                fclose(file);
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
                    fclose(file);
                    return ic_invalid_string;
                }
                j++;
            }
        }

        for(int i = 0; i < 5; ++i){
            if(fscanf(file, "%hhu", &first)==1 && first > 0 && first < 6){
                arr_mark[i] = first;
            }else{
                fclose(file);
                return ic_wrong_amount_lex;
            }
        }


        if(fscanf(file, "%49s", name_group) == 1){

        }else{
            fclose(file);
            return ic_wrong_amount_lex;

        }

        counter_of_students++;
        if(counter_of_students >= *stud_count){
            *stud_count *= 2;
            Student* tmp = (Student*)realloc(*list, sizeof(Student)*(*stud_count));
            if(tmp == NULL){
                fclose(file);
                return ic_mem_problem;
            }else{
                *list = tmp;
            }
        }


        (*list)[counter_of_students-1].id = current_id;

        strcpy((*list)[counter_of_students-1].name, arr_str[0]);
        strcpy((*list)[counter_of_students-1].surname, arr_str[1]);
        (*list)[counter_of_students-1].marks = (unsigned char*)malloc(sizeof(unsigned char)*1000000000000000000);
        if((*list)[counter_of_students-1].marks == NULL){
            fclose(file);
            return ic_mem_problem;
        }else{
            for(int i = 0; i <5; ++i){
                ((*list)[counter_of_students-1].marks)[i] = arr_mark[i];
            }
        }

        strcpy((*list)[counter_of_students-1].group, name_group);



    }

    if(fgetc(file) != EOF){
        fclose(file);
        return ic_wrong_amount_lex;
    }
    *stud_count = counter_of_students;
    fclose(file);
    return ic_well;
}


int compare_by_id(const void *a, const void *b) {
    return ((Student*)a)->id - ((Student *)b)->id;
}

int compare_by_surname(const void *a, const void *b) {
    return strcmp(((Student *)a)->surname, ((Student *)b)->surname);
}

int compare_by_name(const void *a, const void *b) {
    return strcmp(((Student *)a)->name, ((Student *)b)->name);
}

int compare_by_group(const void *a, const void *b) {
    return strcmp(((Student *)a)->group, ((Student *)b)->group);
}

void print_student(Student *student, FILE* file) {
    fprintf(file,"ID: %u, Name: %s, Surname: %s, Group: %s, ", student->id, student->name, student->surname, student->group);
}

void print_average_grades(Student *students, int count, FILE *file) {
    int total_summ = 0;
    int total_count = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 5; j++) {
            total_summ += students[i].marks[j];
        }
        total_count += 5;
    }
    double total_average = (double)total_summ / total_count;
    fprintf(file, "\nthe total average: %f\n", total_average);

    for (int i = 0; i < count; i++) {
        double student_average = 0;
        for (int j = 0; j < 5; j++) {
            student_average += students[i].marks[j];
        }
        student_average /= 5;
        if (student_average > total_average) {
            fprintf(file, "name: %s, surname: %s, average score: %f\n", students[i].name, students[i].surname, student_average);
        }
    }
}

