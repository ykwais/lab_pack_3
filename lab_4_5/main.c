#include "helper.h"

int main(int argc, char** argv) {
    if(argc < 2)
    {
        print_state(invalid_argument, NULL);
        return 0;
    }

    int counter = 0;
    for(int i = 1; i < argc; ++i)
    {
        FILE* file = fopen(argv[i], "r");
        if(file == NULL)
        {
            print_state(problem_with_opening_file, argv[i]);
            return 0;
        }
        char* error_filename = generate_new_filename(argv[i]);
        if(error_filename == NULL)
        {
            fclose(file);
            print_state(memory_problem, NULL);
            return 0;
        }
        printf("new: %s\n", error_filename);
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
            printf("infix: %s\n", line);
            if(!check_brackets(line))
            {
                fclose(file);
                print_state(wrong_brackets, line);
                free(line);
                line = read_line(file, &st);
                continue;
            }
            char* post = NULL;
            st = infix_to_postfix(line, &post);
            if(st == invalid_symbol)
            {
                print_state(invalid_symbol, line);
                free(line);
                line = read_line(file, &st);
                continue;
            }
            if(st != well)
            {
                print_state(invalid_argument, line);
                free(line);
                fclose(file);
                return 0;
            }
            printf("postfix: %s\n", post);

            free(post);
            free(line);
            line = read_line(file, &st);
        }
        if(st == memory_problem)
        {
            fclose(file);
            print_state(memory_problem, NULL);
            return 0;
        }
        if(st == last_str && counter == 0)
        {
            print_state(empty_file, argv[i]);
            fclose(file);
            continue;
        }
        if(st == last_str)
        {
            fclose(file);
            counter = 0;
            continue;
        }

    }

}


void print_state(state st, char* message)
{
    switch(st)
    {
        case invalid_argument:
            printf("Invalid argument\n");
            break;
        case well:
            printf("Well\n");
            break;
        case empty_file:
            printf("Empty file or memory allocation problem: %s\n", message);
            break;
        case memory_problem:
            printf("Memory problem\n");
            break;
        case invalid_symbol:
            printf("Invalid symbol\n");
            break;
        case problem_with_opening_file:
            printf("Problem with opening file: %s\n", message);
            break;
        case wrong_brackets:
            printf("Wrong brackets\n");
            break;
        case not_1_string_in_file:
            printf("there is not 1 string in file, check input file!\n");
            break;
        case wrong_operation:
            printf("Wrong operation in postfix\n");
            break;
        default:
            printf("Unknown state\n");
            break;
    }
}

int priority_of_symbol(char symbol)
{
    switch(symbol)
    {
        case'^':
            return 3;
        case '*':
        case '/':
        case '%':

            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

char* read_line(FILE* file, state* stat)
{
    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL){
        *stat = memory_problem;
        return NULL;
    }
    int ch;
    while((ch = fgetc(file)) != EOF && ch != '\n' && ch != '\r'){
        size++;
        if(size >= buf)
        {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL)
            {
                free(result);
                *stat = memory_problem;
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
    *stat = well;
    return result;
}

bool check_brackets(char* str)
{
    char* ptr = str;
    int brackets = 0;
    while(*ptr != '\0')
    {
        if(*ptr == '(')
        {
            brackets++;
        }else if(*ptr == ')')
        {
            brackets--;
        }
        ptr++;
    }
    if(brackets != 0)
    {
        return false;
    }

    return true;
}

state infix_to_postfix(char* str, char** postfix)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(stack == NULL)
    {
        return memory_problem;
    }
    stack->bottom = NULL;
    stack->top = NULL;

    int size = 0;
    int buf = 1;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL)
    {
        free_stack(stack);
        return memory_problem;
    }
    result[size] = '\0';

    state st;
    char* ptr = str;

    while(*ptr != '\0')
    {
        if(*ptr == ' ' || *ptr == '\t')
        {
            ptr++;
            continue;
        }
        if(isdigit(*ptr))
        {
            while(isdigit(*ptr))
            {
                st = add_symbol_in_array_char(&result, &size, &buf, *ptr);
                if(st != well)
                {
                    free(result);
                    free_stack(stack);
                    return st;
                }
                ptr++;
            }
            st = add_symbol_in_array_char(&result, &size, &buf, ' ');
            if(st != well)
            {
                free(result);
                free_stack(stack);
                return st;
            }
            continue;

        }
        else if (*ptr == '(') {

            st = push(stack, *ptr);
            if (st != well) {
                free(result);
                free_stack(stack);
                return st;
            }
        } else if (*ptr == ')') {

            while (stack->top->symbol != '(') {

                st = add_symbol_in_array_char(&result, &size, &buf, stack->top->symbol);
                if(st != well)
                {
                    free(result);
                    free_stack(stack);
                    return st;
                }
                st = add_symbol_in_array_char(&result, &size, &buf, ' ');
                if(st != well)
                {
                    free(result);
                    free_stack(stack);
                    return st;
                }
                st = pop(stack);
                if (st != well) {
                    free(result);
                    free_stack(stack);
                    return st;
                }
            }
            st = pop(stack);
            if (st != well) {
                free(result);
                free_stack(stack);
                return st;
            }
        }
        else if(*ptr == '+' || *ptr == '-' || *ptr == '/' || *ptr == '%' || *ptr == '*' || *ptr == '^') {

            while (stack->top != NULL && priority_of_symbol(stack->top->symbol) >= priority_of_symbol(*ptr))
            {
                st = add_symbol_in_array_char(&result, &size, &buf, stack->top->symbol);
                if(st != well)
                {
                    free(result);
                    free_stack(stack);
                    return st;
                }
                st = add_symbol_in_array_char(&result, &size, &buf, ' ');
                if(st != well)
                {
                    free(result);
                    free_stack(stack);
                    return st;
                }
                st = pop(stack);
                if (st != well) {
                    free(result);
                    free_stack(stack);
                    return st;
                }
            }
            st = push(stack, *ptr);
            if (st != well) {
                free(result);
                free_stack(stack);
                return st;
            }

        }
        else
        {
            free(result);
            free_stack(stack);
            return invalid_symbol;
        }
        ptr++;
    }


    while(stack->top != NULL)
    {
        if(stack->top->symbol == '(' || stack->top->symbol == ')')
        {
            st = pop(stack);
            if(st != well)
            {
                free(result);
                free_stack(stack);
                return st;
            }
            continue;
        }

        st = add_symbol_in_array_char(&result, &size, &buf, stack->top->symbol);
        if(st != well)
        {
            free(result);
            free_stack(stack);
            return st;
        }
        st = add_symbol_in_array_char(&result, &size, &buf, ' ');
        if(st != well)
        {
            free(result);
            free_stack(stack);
            return st;
        }
        st = pop(stack);
        if(st != well)
        {
            free(result);
            free_stack(stack);
            return st;
        }
    }
    result[size] = '\0';
    *postfix = result;
    free_stack(stack);
    return well;
}

state push(Stack* stack, char symbol)
{
    if(stack->bottom == NULL)
    {
        stack->bottom = (Stack_node*)malloc(sizeof(Stack_node));
        if(stack->bottom == NULL)
        {
            return memory_problem;
        }
        stack->bottom->symbol = symbol;
        stack->bottom->next = NULL;
        stack->top = stack->bottom;
        return well;
    }

    Stack_node* node = (Stack_node*)malloc(sizeof(Stack_node));
    if(node == NULL)
    {
        return memory_problem;
    }

    stack->top->next = node;
    stack->top = node;
    stack->top->symbol = symbol;
    stack->top->next = NULL;

    return well;
}

state pop(Stack* stack)
{
    if(stack->bottom == NULL)
    {
        return invalid_argument;
    }
    if(stack->bottom->next == NULL)
    {
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return well;
    }
    Stack_node* tmp = stack->bottom;
    while(tmp->next->next != NULL)
    {
        tmp = tmp->next;
    }
    stack->top = tmp;

    free(tmp->next);
    tmp->next = NULL;
    return well;
}

void free_stack(Stack* stack)
{
    while(stack->top != NULL)
    {
        pop(stack);
    }
    free(stack);
}

state add_symbol_in_array_char(char** array, int* size, int* buf, char symbol)
{
    if(*size >= *buf - 1)
    {
        *buf *= 2;
        char* tmp = (char*) realloc(*array, sizeof(char)*(*buf+1));
        if(tmp == NULL)
        {
            free(*array);
            return memory_problem;
        }
        *array = tmp;
    }
    (*array)[*size] = symbol;
    (*size)++;
    return well;
}

char* generate_new_filename(char* filename)
{
    int size = 0;
    int buf = 1;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL)
    {
        return NULL;
    }
    result[size] = '\0';
    strcat(result, "error_");
    strcat(result, filename);
    return result;
}