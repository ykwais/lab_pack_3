#include "helper.h"

bool is_operation(char ch)
{
    char ch2 = ch;
    if(ch2 == '+' || ch2 == '-' || ch2 == '*' || ch2 == '/' || ch2 == '%')
    {
        return true;
    }
    return false;

}

state init_memory(memory* storage)
{
    storage->size = 0;
    storage->buf = 2;
    storage->cells = (memory_cell*)malloc(sizeof(memory_cell)*storage->buf);
    if(storage->cells == NULL)
    {
        return meme_problem;
    }
    return well;
}

void free_memory(memory* storage)
{
    for(int i = 0; i < storage->size; i++)
    {
        free(storage->cells[i].var_name);
    }
    free(storage->cells);
}


void print_status_codes(state status)
{
    switch (status)
    {
        case well:
            printf("all well\n");
            break;
        case invalid_input:
            printf("Invalid input\n");
            break;
        case error_open:
            printf("Problem with opening the file\n");
            break;
        case div_zero:
            printf("Division by zero!!!\n");
            break;
        case meme_problem:
            printf("problem with memory allocation!\n");
            break;
        case invalid_argument:
            printf("Invalid argument.\n");
            break;
        case uninit_var:
            printf("Using uninitialized variable or incorrect action.\n");
            break;
        case conv_error:
            printf("Error converting string to number or there is undefined variable.\n");
            break;
        case no_var:
            printf("Variable not found in the memory array or error with digit\n");
            break;
        case empty_file:
            printf("File is empty\n");
            break;
        default:
            printf("smth wrong\n");
            break;
    }
}

void print_var(var_state st, memory_cell* cell, memory* storage, const long long int* value)
{
    switch (st){
        case print_all:
            if(storage->size == 0) {
                printf("Memory is empty\n");
                break;
            }
            for(int i = 0; i < storage->size; i++)
            {
                printf("%s = %lld\n", storage->cells[i].var_name, storage->cells[i].value);
            }
            break;
        case print_one:
            printf("%s = %lld\n", cell->var_name, cell->value);
            break;
        case print_val:
            printf("%lld\n", *value);
            break;
        default:
            printf("smth wrong\n");
            break;
    }
}


memory_cell* find_var(memory* storage, char* var_name)
{
    int left = 0;
    int right = storage->size - 1;
    while(left <= right)
    {
        int mid = (left + right) / 2;
        int cmp = strcmp(storage->cells[mid].var_name, var_name);
        if(cmp == 0)
            return &storage->cells[mid];
        else if(cmp > 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return NULL;
}

state string_to_ll(char* str, long long int* res)
{
    if(strlen(str) > 19) return conv_error;
    char* end;
    long long int val = strtoll(str, &end, 10);
    if(*end != '\0' || end == str) return conv_error;
    *res = val;
    return well;

}

state add_to_memory(memory* storage, char* var_name, long long int value)
{
    memory_cell* there_is = find_var(storage, var_name);
    if(there_is != NULL)
    {
        there_is->value = value;
        return well;
    }

    if(storage->size >= storage->buf - 1)
    {
        storage->buf *= 2;
        memory_cell* tmp = (memory_cell*)realloc(storage->cells, sizeof(memory_cell)*storage->buf);
        if(tmp == NULL) return meme_problem;
        storage->cells = tmp;
    }

    int len_name = (int)strlen(var_name);
    storage->cells[storage->size].var_name = (char*)malloc(sizeof(char)*(len_name + 1));
    if(storage->cells[storage->size].var_name == NULL) return meme_problem;
    strcpy(storage->cells[storage->size].var_name, var_name);

    storage->cells[storage->size].value = value;
    storage->size++;
    qsort(storage->cells, storage->size, sizeof(memory_cell), compare_var_name);
    return well;

}


int compare_var_name(const void* first_name, const void* second_name)
{
    return strcmp(((memory_cell*)first_name)->var_name, ((memory_cell*)second_name)->var_name);
}

operation_type get_operation(char symbol)
{
    switch(symbol)
    {
        case '+':
            return add;
        case '-':
            return sub;
        case '*':
            return mul;
        case '/':
            return division;
        case '%':
            return mod;
        case '=':
            return eql;
        default:
            return undef;
    }
}

state make_operation(long long int* res, long long int first, long long int second, operation_type operation)
{
    switch (operation) {
        case add:
            *res = first + second;
            return well;
        case sub:
            *res = first - second;
            return well;
        case mul:
            *res = first * second;
            return well;
        case division:
            if (second == 0) return div_zero;
            *res = first / second;
            return well;
        case mod:
            if (second == 0) return div_zero;
            *res = first % second;
            return well;
        default:
            return invalid_input;
    }
}

void free_4_arr(char** first_arg, char** second_arg, char** operation, char** var_name)
{
    if(*first_arg != NULL)
    {
        free(*first_arg);
    }
    if(*second_arg != NULL)
    {
        free(*second_arg);
    }
    if(*operation != NULL)
    {
        free(*operation);
    }
    if(*var_name != NULL)
    {
        free(*var_name);
    }

}

state parsing_file(FILE* file, memory* storage)
{
    if(file == NULL || storage == NULL) {
        return invalid_argument;
    }
    int ch;
    if((ch = fgetc(file))== EOF)
    {
        return empty_file;
    }

    while(ch != EOF)
    {
        if(ch == ';' || isspace(ch))
        {
            ch = fgetc(file);
            continue;
        }
        ungetc(ch, file);

        int buf = 2;
        char* var_name = (char*)malloc(sizeof(char)*(buf + 1));
        if(var_name == NULL)
        {
            return meme_problem;
        }
        var_name[0] = '\0';
        char* operation = (char*)malloc(sizeof(char)*(buf + 1));
        if(operation == NULL)
        {
            free(var_name);
            return meme_problem;
        }
        operation[0] = '\0';
        char* first_arg = (char*)malloc(sizeof(char)*(buf + 1));
        if(first_arg == NULL)
        {
            free(var_name);
            free(operation);
            return meme_problem;
        }
        first_arg[0] = '\0';
        char* second_arg = (char*)malloc(sizeof(char)*(buf + 1));
        if(second_arg == NULL)
        {
            free(var_name);
            free(operation);
            free(first_arg);
            return meme_problem;
        }
        second_arg[0] = '\0';



        state st;

        st = get_instruction(file, &var_name, &operation, &first_arg, &second_arg);
        if(st != well)
        {
            free_4_arr(&first_arg, &second_arg, &operation, &var_name);
            return st;
        }

        char op_ch = operation[0];

        if(strcmp(var_name, "print") == 0 )
        {
            if(strlen(first_arg) == 0 && strlen(second_arg) == 0)
            {
                print_var(print_all, NULL, storage, NULL);
            }
            else
            {
                long long int current;
                if(strlen(second_arg) == 0)
                {
                    memory_cell* current_cell = find_var(storage, first_arg);
                    if(current_cell != NULL)
                        print_var(print_one, current_cell, NULL, NULL);
                    else
                    {
                        st = string_to_ll(first_arg, &current);
                        if(st != well)
                        {
                            free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                            return conv_error;
                        }
                        print_var(print_val, NULL, NULL, &current);
                    }
                }
                else
                {
                    free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                    return invalid_input;
                }
            }
        }

        if(operation[0] == '=')
        {
            long long int current = -1;
            memory_cell* current_cell = find_var(storage, first_arg);
            if(current_cell != NULL) current = current_cell->value;
            else
            {
                st = string_to_ll(first_arg, &current);
                if(st != well)
                {
                    free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                    return uninit_var;
                }
            }
            st = add_to_memory(storage, var_name, current);
            if(st != well)
            {
                free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                return st;
            }
        }
        else if(is_operation(op_ch))
        {
            long long int first_value = -1, second_value = -1;
            memory_cell* first_cell = find_var(storage, first_arg);
            if(first_cell != NULL) first_value = first_cell->value;
            else
            {
                st = string_to_ll(first_arg, &first_value);
                if(st != well)
                {
                    free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                    return conv_error;
                }
            }

            memory_cell* second_cell = find_var(storage, second_arg);
            if(second_cell != NULL) second_value = second_cell->value;
            else
            {
                st = string_to_ll(second_arg, &second_value);
                if(st != well)
                {
                    free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                    return uninit_var;
                }
            }

            operation_type ot = get_operation(operation[0]);
            if(ot != undef)
            {
                long long int new_value = -1;
                st = make_operation(&new_value, first_value, second_value, ot);
                if(st != well)
                {
                    free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                    return st;
                }
                st = add_to_memory(storage, var_name, new_value);
                if(st != well)
                {
                    free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                    return st;
                }
            }
            else
            {
                free_4_arr(&first_arg, &second_arg, &operation, &var_name);
                return invalid_input;
            }
        }
        else{}

        free_4_arr(&first_arg, &second_arg, &operation, &var_name);
        ch = fgetc(file);
    }

    return well;
}

state get_instruction(FILE* file, char** name, char** operation, char** first_arg, char** second_arg)
{
    int name_size = 0, name_buf = 2, first_arg_size = 0, first_arg_buf = 2, second_arg_size = 0, second_arg_buf = 2;
    state st;
    int ch;

    while((ch = fgetc(file)) != EOF && (isspace(ch) || ch == ';'));

    while(isalpha(ch))
    {
        if(name_size >= name_buf - 1)
        {
            name_buf *= 2;
            char* tmp = (char*)realloc(*name, sizeof(char)*(name_buf + 1)) ;
            if(tmp == NULL) return meme_problem;
            *name = tmp;
        }
        (*name)[name_size++] = (char)ch;
        ch = fgetc(file);
    }
    (*name)[name_size] = '\0';

    while(isspace(ch) && ch != EOF)
    {
        ch = fgetc(file);
    }

    if(ch == '=')
    {
        (*operation)[0] = '=';
        (*operation)[1] = '\0';
        ch = fgetc(file);
        while(isspace(ch))
        {
            ch = fgetc(file);
        }
    }
    else
    {
        if(strcmp(*name, "print") == 0)
        {
            if(ch == ';')
            {
                (*operation)[0] = '\0';
                (*first_arg)[0] = '\0';
                (*second_arg)[0] = '\0';
                return well;
            }
            while(1)
            {
                if(ch == ';' || ch == EOF || isspace(ch)) break;
                if(first_arg_size >= first_arg_buf - 1)
                {
                    first_arg_buf *= 2;
                    char* tmp = (char*)realloc(*first_arg, sizeof(char)*(first_arg_buf + 1)) ;
                    if(tmp == NULL) {
                        return meme_problem;
                    }
                    *first_arg = tmp;
                }
                if(is_operation((char)ch) && !isalpha(ch) && !isdigit(ch) && ch != '-')
                {
                    return invalid_input;
                }

                (*first_arg)[first_arg_size++] = (char)ch;
                ch = fgetc(file);
            }
            (*first_arg)[first_arg_size] = '\0';

            while(isspace(ch))
            {
                ch = fgetc(file);
            }
            if( ch != ';') {//eof
                return invalid_input;
            }
            (*second_arg)[0] = '\0';
            return well;
        }
        else {
            return invalid_input;
        }
    }


    while(isdigit(ch) || isalpha(ch) || ch == '-')
    {

        if(first_arg_size >= first_arg_buf - 1)
        {
            first_arg_buf *= 2;
            char* tmp = (char*)realloc(*first_arg, sizeof(char)*(first_arg_buf + 1)) ;
            if(tmp == NULL) {
                return meme_problem;
            }
            *first_arg = tmp;
        }
        (*first_arg)[first_arg_size++] = (char)ch;
        ch = fgetc(file);
    }
    (*first_arg)[first_arg_size] = '\0';

    while(isspace(ch))
    {
        ch = fgetc(file);
    }
    if(ch == ';') {
        (*second_arg)[0] = '\0';
        return well;
    }
    else
    {
        if(!is_operation((char)ch))
        {
            return invalid_input;
        }

        (*operation)[0] = (char)ch;
        (*operation)[1] = '\0';
    }
    ch = fgetc(file);
    while(isspace(ch))
    {
        ch = fgetc(file);
    }

    int counter_minus = 0;
    while(ch != ' ' && ch != ';' && ch != '\n' && ch != '\t' && ch != '\r')
    {
        if(ch == '-') counter_minus++;
        if(second_arg_size >= second_arg_buf - 1)
        {
            second_arg_buf *= 2;
            char* tmp = (char*)realloc(*second_arg, sizeof(char)*(second_arg_buf + 1)) ;
            if(tmp == NULL) {
                return meme_problem;
            }
            *second_arg = tmp;
        }
        if(is_operation((char)ch) && !isalpha(ch) && !isdigit(ch) && (ch == '-' && counter_minus != 1))
        {
            return invalid_input;
        }

        (*second_arg)[second_arg_size++] = (char)ch;
        ch = fgetc(file);
    }
    (*second_arg)[second_arg_size] = '\0';

    while(isspace(ch))
    {
        ch = fgetc(file);
    }
    if(ch == ';') return well;
    else
    {
        return invalid_input;
    }

}