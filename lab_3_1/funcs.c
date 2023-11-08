#include "helper.h"

cc kind_cc(int base, int* mask, int* in_left)
{
    switch (base) {
        case 2:
            *mask = 1;
            *in_left = 1;
            return base_well;
        case 4:
            *mask = 3;
            *in_left = 2;
            return base_well;
        case 8:
            *mask = 7;
            *in_left = 3;
            return base_well;
        case 16:
            *mask = 15;
            *in_left = 4;
            return base_well;
        case 32:
            *mask = 31;
            *in_left = 5;
            return base_well;
        default:
            return base_invalid;
    }
}


memo number_to_cc(int number, int base, int* mask, int* in_left, char** result)
{
    char* answer = (char*) malloc(sizeof(char)*2);
    int length = 2;
    int counter = 0;
    int start = 0;


    switch(kind_cc(base, mask ,in_left)){
        case base_invalid:
            return mem_invalid_base;
        case base_well:
            break;
    }

    if(number == 0){
        answer[0] = '0';
        answer[1] = '\0';
        *result = answer;
        return mem_well;
    }

    if(number < 0){
        number *= -1;
        answer[counter] = '-';
        counter++;
        start = 1;
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

        answer[counter-1] = digit > 9 ? (char)(digit - 10 + 'A') : (char)(digit + '0');

    }
    reverse_string(&answer, start, counter);
    answer[counter] = '\0';

    *result = answer;
    return mem_well;
}


void reverse_string(char** string, int start, int length){
    int i = 0;
    int j = 0;
    for ( i = start,  j = length - 1; i < j; i++, j--)
    {
        int tmp = (*string)[i];
        (*string)[i] = (*string)[j];
        (*string)[j] = tmp;
    }
}
