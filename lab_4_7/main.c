#include "helper.h"


int main(int argc, char** argv) {
    if(argc != 2)
    {
        print_status_codes(invalid_input);
        return 0;
    }

    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        print_status_codes(error_open);
        return 0;
    }

    state st;
    memory storage_var;

    st = init_memory(&storage_var);
    if(st != well)
    {
        free_memory(&storage_var);
        print_status_codes(st);
        fclose(file);
        return 0;
    }

    st = parsing_file(file, &storage_var);
    if(st != well)
    {
        free_memory(&storage_var);
        print_status_codes(st);
        fclose(file);
        return 0;
    }
    free_memory(&storage_var);
    fclose(file);
}




