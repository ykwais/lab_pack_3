#include "helper.h"

int main() {
    int mask = 0;
    int in_left = 0;
    int num = -123;////////////////////
    int base = 17;//////////////////////
    char* result = NULL;

    switch(number_to_cc(num, base, &mask, &in_left, &result)){
        case mem_bad:
            printf("problem with memory allocation!\n");
            return 0;
        case mem_invalid_base:
            printf("You've inputted the wrong base!\n");
            return 0;
        case mem_well:
            printf("%d in cc_%d : ", num, base);
            printf("%s", result);
            break;
    }
    free(result);
}