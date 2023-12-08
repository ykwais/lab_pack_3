#include "helper.h"

int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("Error: wrong number of arguments\n");
    }
    if(strlen(argv[2]) != 1)
    {
        printf("Error: wrong variable\n");
    }

    state st = task(argv[1], argv[2][0]);
    print_status_codes(st);
}