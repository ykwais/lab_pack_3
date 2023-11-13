#include "helper.h"

cc kind_cc(int base, int* mask, int* in_left)
{
    if(base != 2 && base !=  4 && base != 8 && base != 16 && base != 32){
        return base_invalid;
    }
    int two_value = 1;
    int pow_value = 0;
    while(two_value < base)
    {
        two_value <<= 1;
        pow_value = add(pow_value, 1);
    }
    if(two_value == base){
        *mask = add(two_value, negative(1));
        *in_left = pow_value;
        return base_well;
    }
    return base_invalid;
}


memo number_to_cc(int number, int base, int* mask, int* in_left, char** result)
{
    char* answer = (char*)calloc(2,sizeof(char));
    if(answer == NULL){
        return mem_bad;
    }

    int length = 2;
    int counter = 0;
    int start = 0;


    switch(kind_cc(base, mask ,in_left)){
        case base_invalid:
            free(answer);
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
        number = negative(number);//*-1
        answer[counter] = '-';
        counter = add(counter, 1);
        start = 1;
    }

    while(number > 0){
        counter = add(counter, 1);
        if( counter >= length){
            length <<= 2;
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

        answer[add(counter,negative(1))] = digit > 9 ? (char)(add(add(digit, negative(10)), 'A')) : (char)(add(digit,'0'));

    }
    reverse_string(&answer, start, counter);
    answer[counter] = '\0';

    *result = answer;
    return mem_well;
}


void reverse_string(char** string, int start, int length){
    int i = 0;
    int j = 0;
    for ( i = start, j = add(length,negative(1)); i < j; i = add(i, 1), j = add(j, negative(1)))
    {
        int tmp = (*string)[i];
        (*string)[i] = (*string)[j];
        (*string)[j] = tmp;
    }
}

int add(int x, int y) {

    while (y != 0) {
        int carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

int negative(int x){
    return add(~x, 1);
}

