#include "helper_2.h"


input_status collect_data(char* filename, Student** list, int* stud_count, int* buf) {
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

    int counter_of_students = 0;
    state st = well;
    char *one_full_string = NULL;
    while ((one_full_string = read_line(&st, file)) != NULL) {

        char *tmp = (char *) malloc(sizeof(char) * (strlen(one_full_string) + 1));
        if (tmp == NULL) {
            free(one_full_string);
            fclose(file);
            *stud_count = counter_of_students;
            return ic_mem_problem;
        }
        strcpy(tmp, one_full_string);

        int count_token_in_full = count_tokens(tmp, " ");
        if (count_token_in_full != 9) {
            fclose(file);
            free(one_full_string);
            free(tmp);
            *stud_count = counter_of_students;
            return ic_wrong_amount_lex;
        }
        free(tmp);


        if(counter_of_students >= *buf){
            *buf *= 2;
            Student* temp = (Student*)realloc(*list, sizeof(Student)*(*buf));
            if(temp == NULL){
                fclose(file);
                free(one_full_string);
                *stud_count = counter_of_students;
                return ic_mem_problem;
            }else{
                *list = temp;
            }
        }

        (*list)[counter_of_students].name = NULL;
        (*list)[counter_of_students].surname = NULL;
        (*list)[counter_of_students].group = NULL;
        (*list)[counter_of_students].marks = NULL;

        char* uid = strtok(one_full_string, " ");
        uid = create_string(uid);
        if(uid == NULL)
        {
            free(one_full_string);
            fclose(file);
            *stud_count = counter_of_students;
            return ic_mem_problem;
        }
        char* end;
        ui id = strtoul(uid, &end, 10);
        if (end == uid || uid[0] == '-' || *end != '\0' || strlen(uid) > 10) {
            fclose(file);
            free(one_full_string);
            free(uid);
            return ic_invalid_string;
        }
        free(uid);
        for(int i = 0; i < counter_of_students; ++i)
        {
            if(id == (*list)[i].id){
                fclose(file);
                free(one_full_string);
                *stud_count = counter_of_students;
                return ic_the_same_id;
            }
        }

        (*list)[counter_of_students].id = id;


        char* name = strtok(NULL, " ");
        name = create_string(name);
        if(name == NULL)
        {
            fclose(file);
            free(one_full_string);
            return ic_mem_problem;
        }
        (*list)[counter_of_students].name = name;


        char* surname = strtok(NULL, " ");
        surname = create_string(surname);
        if(surname == NULL)
        {
            fclose(file);
            free(one_full_string);
            delete_student(&(*list)[counter_of_students]);
            return ic_mem_problem;
        }
        (*list)[counter_of_students].surname = surname;

        char* group = strtok(NULL, " ");
        group = create_string(group);
        if(group == NULL)
        {
            fclose(file);
            free(one_full_string);
            delete_student(&(*list)[counter_of_students]);
            return ic_mem_problem;
        }
        (*list)[counter_of_students].group = group;

        unsigned char arr_mark[5];

        for(int i = 0; i < 5; ++i)
        {
            char* mark = strtok(NULL, " ");
            if((strlen(mark) > 1 && (mark[1] != '\r'))|| mark[0] > '5' || mark[0] < '1')
            {
                fclose(file);
                free(one_full_string);
                delete_student(&(*list)[counter_of_students]);
                return ic_invalid_string;
            }
            arr_mark[i] = (unsigned char)mark[0];
        }
        free(one_full_string);

        (*list)[counter_of_students].marks = (unsigned char*) malloc(sizeof(unsigned char)*5);
        if((*list)[counter_of_students].marks == NULL)
        {
            fclose(file);
            delete_student(&(*list)[counter_of_students]);
            return ic_mem_problem;
        }


        for(int i = 0; i <5; ++i){
            ((*list)[counter_of_students].marks)[i] = arr_mark[i];
        }

        if(!is_latin_str((*list)[counter_of_students].name) || !is_latin_str((*list)[counter_of_students].surname))
        {
            fclose(file);
            delete_student(&(*list)[counter_of_students]);
            return ic_invalid_string;
        }

        counter_of_students++;
        *stud_count = counter_of_students;

    }
    if (st == meme_problem) {
        fclose(file);
        *stud_count = counter_of_students;
        return ic_mem_problem;
    }

    if(st == empty_str){
        fclose(file);
        *stud_count = counter_of_students;
        return ic_empty_str;
    }
    *stud_count = counter_of_students;
    fclose(file);
    return ic_well;
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
        *stat = last_str;
        free(result);
        return NULL;
    }
    result[size] = '\0';
    if(strcmp(result, "\0") == 0 || strcmp(result, "\r") == 0 || strcmp(result, "\n") == 0){
        *stat = empty_str;
        free(result);
        return NULL;
    }
    *stat = well;
    return result;
}

int count_tokens(char *str, const char *delim) {
    int count = 0;
    char *token = strtok(str, delim);
    while (token != NULL) {
        count++;
        token = strtok(NULL, delim);
    }
    return count;
}

void delete_student(Student* student)
{
    if(student->name != NULL){
        free(student->name);
        student->name = NULL;
    }
    if(student->surname != NULL){
        free(student->surname);
        student->surname = NULL;
    }
    if(student->group != NULL){
        free(student->group);
        student->group = NULL;
    }
    if(student->marks != NULL){
        free(student->marks);
        student->marks = NULL;
    }

}

char* create_string(char* str)
{
    if(str == NULL){
        return NULL;
    }
    int len = (int)strlen(str);
    char* result = (char*)malloc(sizeof(char)*(len+1));
    if(result == NULL){
        return NULL;
    }
    strcpy(result, str);
    if(result == NULL){
        free(result);
        return NULL;
    }
    return result;

}

bool is_latin_str(char* str)
{
    if(str == NULL){
        return false;
    }
    int len = (int)strlen(str);
    for(int i = 0; i < len; ++i){
        if(!isalpha(str[i])){
            return false;
        }
    }
    return true;

}

void free_students(Student** lst, const int* count)
{
    for(int i = 0; i < *count; ++i)
    {
        delete_student(&((*lst)[i]));
    }
    free(*lst);

}

void print_student(Student *student, FILE* file)
{
    fprintf(file,"ID: %u, Name: %s, Surname: %s, Group: %s, \n", student->id, student->name, student->surname, student->group);
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


void print_average_grades(Student *students, int count, FILE *file) {
    int total_summ = 0;
    int total_count = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 5; j++) {
            total_summ += students[i].marks[j] - '0';
        }
        total_count += 5;
    }
    double total_average = (double)total_summ / total_count;
    fprintf(file, "\nthe total average: %f\n", total_average);

    for (int i = 0; i < count; i++) {
        double student_average = 0;
        for (int j = 0; j < 5; j++) {
            student_average += students[i].marks[j] - '0';
        }
        student_average /= 5;
        if (student_average > total_average) {
            fprintf(file, "name: %s, surname: %s, average score: %f\n", students[i].name, students[i].surname, student_average);
        }
    }
}
