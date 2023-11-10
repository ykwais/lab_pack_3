

#ifndef LAB_3_1_HELPER_H
#define LAB_3_1_HELPER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum base_type{
    base_invalid,
    base_well
} cc;

typedef enum mem_problem{
    mem_well,
    mem_bad,
    mem_invalid_base
}memo;

cc kind_cc(int base, int* mask, int* in_left);
memo number_to_cc(int number, int base, int* mask, int* in_left, char** result);
void reverse_string(char** string, int start, int length);
int add(int x, int y);

#endif //LAB_3_1_HELPER_H
