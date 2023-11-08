#include <stdio.h>
#include <stdlib.h>

//typedef enum base_type{
//    invalid,
//    type_2,
//    type_4,
//    type_8,
//    type_16,
//    type_32
//} cc;

typedef enum mem_problem{
    mem_well,
    mem_bad
}memo;

void kind_cc(int base, int* mask, int* in_left);
memo number_to_cc(int number, int base, int* mask, int* in_left, char** result);

int main() {
    int mask = 0;
    int in_left = 0;
    int num = 11;
    int base = 4;
    char* result = NULL;

    switch(number_to_cc(num, base, &mask, &in_left, &result)){
        case mem_bad:
            break;
        case mem_well:
            printf("%d in cc_%d : ", num, base);
            int i = 0;
            while(result[i] != '\0'){
                printf("%c", result[i]);
                i++;
            }
            break;
    }


    free(result);
}

void kind_cc(int base, int* mask, int* in_left)
{
    switch (base) {
        case 2:
            *mask = 1;
            *in_left = 1;
            return;
        case 4:
            *mask = 3;
            *in_left = 2;
            return;
        case 8:
            *mask = 7;
            *in_left = 3;
            return;
        case 16:
            *mask = 15;
            *in_left = 4;
            return;
        case 32:
            *mask = 31;
            *in_left = 5;
            return;
        default:
            return;
    }
}


memo number_to_cc(int number, int base, int* mask, int* in_left, char** result)
{
    char* answer = (char*) malloc(sizeof(char)*2);
    int length = 2;
    int counter = 0;
    kind_cc(base, mask ,in_left);
    if(number < 0){
        number *= -1;
    }

    while(number > 0){
        counter++;
        if( counter >= length){
            length *= 2;
            char* tmp = realloc(answer, length);
            if(tmp == NULL){
                free(answer);
                return mem_bad;
            }else{
                answer = tmp;
            }
        }
        int digit = *mask & number;
        number >>= *in_left;
        //printf("%d", digit);
        answer[counter-1] = digit > 9 ? (char)(digit - 10 + 'A') : (char)(digit + '0');

    }
    answer[counter] = '\0';
    *result = answer;
    return mem_well;
}

