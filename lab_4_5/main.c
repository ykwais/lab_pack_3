#include "helper.h"



state push_val(Val_stack* stack, long long int val)
{
    if(stack->bottom == NULL)
    {
        stack->bottom = (Val_node*)malloc(sizeof(Val_node));
        if(stack->bottom == NULL)
        {
            return memory_problem;
        }
        stack->bottom->value = val;
        stack->bottom->next = NULL;
        stack->top = stack->bottom;
        return well;
    }
    Val_node* node = (Val_node*)malloc(sizeof(Val_node));
    if(node == NULL)
    {
        return memory_problem;
    }
    stack->top->next = node;
    stack->top = node;
    stack->top->value = val;
    stack->top->next = NULL;
    return well;
}

state pop_val(Val_stack* stack)
{
    if(stack->bottom == NULL)
    {
        return invalid_symbol;
    }
    if(stack->bottom->next == NULL)
    {
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return well;
    }
    Val_node* tmp = stack->bottom;
    while(tmp->next->next != NULL)
    {
        tmp = tmp->next;
    }
    stack->top = tmp;
    free(tmp->next);
    tmp->next = NULL;
    return well;
}

void free_val_stack(Val_stack* stack)
{
    while(stack->top != NULL)
    {
        pop_val(stack);
    }
    free(stack);
}


int main(int argc, char** argv) {
    if(argc < 2)
    {
        print_state(invalid_argument, NULL, stdout, NULL, NULL, NULL, NULL);
        return 0;
    }

    int counter = 0;
    int counter_writting_file = 0;
    for(int i = 1; i < argc; ++i)
    {
        FILE* file = fopen(argv[i], "r");
        if(file == NULL)
        {
            print_state(problem_with_opening_file, argv[i], stdout, NULL, NULL, NULL, NULL);
            return 0;
        }
        char* error_filename = generate_new_filename(argv[i]);
        if(error_filename == NULL)
        {
            fclose(file);
            print_state(memory_problem, NULL, stdout, NULL, NULL, NULL, NULL);
            return 0;
        }

        FILE* error_file = fopen(error_filename, "w");
        if(error_file == NULL)
        {
            fclose(file);
            print_state(problem_with_opening_file, error_filename, stdout, NULL, NULL, NULL, NULL);
            free(error_filename);
            return 0;
        }

        state st;
        char* line = read_line(file, &st);
        while(line != NULL && st == well )
        {
            if(strlen(line) < 1)
            {
                free(line);
                line = read_line(file, &st);
                continue;
            }
            counter++;

            if(!check_brackets(line))
            {
                int tmp = counter - 1;
                print_state(wrong_brackets, argv[i], error_file, &tmp, line, NULL, NULL);
                counter_writting_file++;
                free(line);
                line = read_line(file, &st);
                continue;
            }
            char* post = NULL;
            st = infix_to_postfix(line, &post);
            if(st == invalid_symbol)
            {
                int tmp = counter - 1;
                print_state(invalid_symbol, argv[i], error_file, &tmp, line, NULL, NULL);
                counter_writting_file++;
                free(line);
                line = read_line(file, &st);
                continue;
            }
            else if(st != well)
            {
                print_state(memory_problem, NULL,stdout, NULL, NULL, NULL, NULL);
                free(line);
                fclose(file);
                fclose(error_file);
                remove(error_filename);
                free(error_filename);
                return 0;
            }

            long long int result = 0;
            st = calculate_postfix(post, &result);
            if(st == overflow)
            {
                print_state(overflow, argv[i],error_file, &counter, line, post, NULL);
                counter_writting_file++;
                free(line);
                free(post);
                line = read_line(file, &st);
                continue;
            }
            if(st == memory_problem)
            {
                print_state(memory_problem, NULL, stdout, NULL, NULL, NULL, NULL);
                free(line);
                fclose(file);
                fclose(error_file);
                if(counter_writting_file == 0)
                {
                    remove(error_filename);
                }
                free(error_filename);
                return 0;
            }

            if(st == invalid_symbol)
            {
                print_state(invalid_symbol, argv[i], error_file, &counter, line, post, NULL);
                counter_writting_file++;
                free(line);
                free(post);
                line = read_line(file, &st);
                continue;
            }

            if(st == division_by_zero)
            {
                print_state(division_by_zero, argv[i], error_file, &counter, line, post, NULL);
                counter_writting_file++;
                free(line);
                free(post);
                line = read_line(file, &st);
                continue;
            }

            print_state(well, argv[i], stdout, NULL, line, post, &result);
            counter_writting_file++;

            free(post);
            free(line);
            line = read_line(file, &st);
        }
        if(st == memory_problem)
        {
            fclose(error_file);
            fclose(file);
            free(error_filename);
            print_state(memory_problem, NULL, stdout, NULL, NULL, NULL, NULL);
            return 0;
        }
        if(st == last_str && counter == 0)
        {
            print_state(empty_file, argv[i], stdout, NULL, NULL, NULL, NULL);
            fclose(file);
            fclose(error_file);
            remove(error_filename);
            free(error_filename);
            continue;
        }
        if(st == last_str)
        {
            fclose(file);
            counter = 0;
            fclose(error_file);
            if(counter_writting_file == 0)
            {
                remove(error_filename);
            }
            free(error_filename);
            counter_writting_file = 0;
            continue;
        }

    }

}



state calculate_postfix(char* postfix, long long int* result)
{
    Val_stack* stack = (Val_stack*)malloc(sizeof(Val_stack));
    if(stack == NULL)
    {
        return memory_problem;
    }
    stack->bottom = NULL;
    stack->top = NULL;

    state st;

    char* token = strtok(postfix, " ");
    while(token != NULL)
    {
        long long int num = 0;
        if(isdigit(token[0]))
        {
            st = stringToLongLong(token, &num);
            if(st != well)
            {
                free_val_stack(stack);
                return st;
            }
            st = push_val(stack, num);
            if(st != well)
            {
                free_val_stack(stack);
                return st;
            }
        }
        else
        {
            int len1 ;
            int len2 ;
            if(stack->top == NULL)
            {
                free_val_stack(stack);
                return invalid_symbol;
            }
            long long int second = stack->top->value;
            st = pop_val(stack);
            if(st != well)
            {
                free_val_stack(stack);
                return st;
            }
            if((len2 = getNumberLength(second)) > 17)
            {
                free_val_stack(stack);
                return overflow;
            }
            if(stack->top == NULL)
            {
                free_val_stack(stack);
                return invalid_symbol;
            }
            long long int first = stack->top->value;
            st = pop_val(stack);
            if(st != well)
            {
                free_val_stack(stack);
                return st;
            }
            if((len1 = getNumberLength(first)) > 17)
            {
                free_val_stack(stack);
                return overflow;
            }
            switch(token[0]) {
                case '+':
                    if(len1 + len2 > 33)
                    {
                        free_val_stack(stack);
                        return overflow;
                    }
                    num = first + second;
                    break;
                case '-':
                    if(len1 + len2 > 33)
                    {
                        free_val_stack(stack);
                        return overflow;
                    }
                    num = first - second;
                    break;
                case '*':
                    if(len1 + len2 > 17)
                    {
                        free_val_stack(stack);
                        return overflow;
                    }
                    num = first * second;
                    break;
                case '/':
                    if(second == 0)
                    {
                        free_val_stack(stack);
                        return division_by_zero;
                    }
                    num = first / second;
                    break;
                case '^':
                    if(second < 0)
                    {
                        free_val_stack(stack);
                        return invalid_symbol;
                    }
                    if(len1 + len2 > 8)
                    {
                        free_val_stack(stack);
                        return overflow;
                    }
                    num = bpow(first, second);
                    break;
                case '%':
                    if(second == 0)
                    {
                        free_val_stack(stack);
                        return division_by_zero;
                    }
                    num = first % second;
                    break;
                default:
                    free_val_stack(stack);
                    return invalid_symbol;
            }
            st = push_val(stack, num);
            if(st != well)
            {
                free_val_stack(stack);
                return st;
            }

        }

        token = strtok(NULL, " ");
    }
    if(stack->top != stack->bottom)
    {
        free_val_stack(stack);
        return invalid_symbol;
    }
    *result = stack->top->value;

    free_val_stack(stack);
    return well;
}


int getNumberLength(long long int number)
{
    int length = 0;

    if (number == 0) {
        return 1;
    }

    if (number < 0) {
        number = -number;
    }

    while (number > 0) {
        length++;
        number /= 10;
    }

    return length;
}

state stringToLongLong(const char* str, long long int* res)
{
    char* endptr;
    errno = 0;
    long long int result = strtoll(str, &endptr, 10);


    if ((errno == ERANGE && (result == LLONG_MAX || result == LLONG_MIN)) || (errno != 0 && result == 0)) {
        return invalid_symbol;
    }

    if (endptr == str || *endptr != '\0') {
        return invalid_symbol;
    }

    *res = result;
    return well;

}

long long int bpow(long long int a, long long int b)
{
    if(b == 0)
    {
        return 1;
    }
    if(b % 2 == 1)
    {
        return a * bpow(a, b - 1);
    }
    long long int tmp = bpow(a, b / 2);
    return tmp * tmp;
}