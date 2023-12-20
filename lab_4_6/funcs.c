#include "helper.h"

void print_state(state st)
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
            printf("Empty file or memory allocation problem\n");
            break;
        case memory_problem:
            printf("Memory problem\n");
            break;
        case invalid_symbol:
            printf("Invalid symbol\n");
            break;
        case problem_with_opening_file:
            printf("Problem with opening file\n");
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

char* read_line(FILE* file)
{
    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL){
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
                return NULL;
            }else{
                result = tmp;
            }
        }
        result[size-1] = (char)ch;
    }
    if(size == 0 && ch == EOF){

        free(result);
        return NULL;
    }
    result[size] = '\0';

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
    bool is_double = false;
    while(*ptr != '\0') {
        if(*ptr == ' ' || *ptr == '\t')
        {
            ptr++;
            continue;
        }
        if((isalpha(*ptr) || *ptr == '1' || *ptr == '0') && !is_double)
        {
            is_double = true;
            if(size >= buf - 1)
            {
                buf *= 2;
                char* tmp = (char*) realloc(result, sizeof(char) * (buf + 1));
                if (tmp == NULL) {
                    free(result);
                    free_stack(stack);
                    return memory_problem;
                }
                result = tmp;
            }
            result[size] = *ptr;
            size++;
        }
        else if (*ptr == '(') {
            is_double = false;
            st = push(stack, *ptr);
            if (st != well) {
                free(result);
                free_stack(stack);
                return st;
            }
        } else if (*ptr == ')') {
            is_double = false;
            while (stack->top->symbol != '(') {

                if (size >= buf - 1) {
                    buf *= 2;
                    char *tmp = (char *) realloc(result, sizeof(char) * (buf + 1));
                    if (tmp == NULL) {
                        free(result);
                        free_stack(stack);
                        return memory_problem;
                    }
                    result = tmp;
                }
                result[size] = stack->top->symbol;
                size++;
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
        else if(*ptr == '+' || *ptr == '-' || *ptr == '<') {
            is_double = false;
            char *tmp = ptr;
            tmp++;
            if (*tmp != '>') {
                free(result);
                free_stack(stack);
                return invalid_symbol;
            }
            while (stack->top != NULL && priority_of_symbol(stack->top->symbol) >= priority_of_symbol(*ptr))
            {
                if (size >= buf - 1) {
                    buf *= 2;
                    char *temp = (char *) realloc(result, sizeof(char) * (buf + 1));
                    if (temp == NULL) {
                        free(result);
                        free_stack(stack);
                        return memory_problem;
                    }
                    result = temp;
                }
                result[size] = stack->top->symbol;
                size++;
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
            ptr++;
        }
        else if(*ptr == '&' || *ptr == '|' || *ptr == '~' || *ptr == '=' || *ptr == '!' || *ptr == '?')
        {
            is_double = false;
            while(stack->top != NULL && priority_of_symbol(stack->top->symbol) >= priority_of_symbol(*ptr))
            {
                if(size >= buf - 1)
                {
                    buf *= 2;
                    char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
                    if(tmp == NULL)
                    {
                        free(result);
                        free_stack(stack);
                        return memory_problem;
                    }
                    result = tmp;
                }
                result[size] = stack->top->symbol;
                size++;
                st = pop(stack);
                if(st != well)
                {
                    free(result);
                    free_stack(stack);
                    return st;
                }
            }
            st = push(stack, *ptr);
            if(st != well)
            {
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
                return st;
            }
        }

        if(size >= buf - 1)
        {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL)
            {
                free(result);
                free_stack(stack);
                return memory_problem;
            }
            result = tmp;
        }
        result[size] = stack->top->symbol;
        size++;
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

int priority_of_symbol(char symbol)
{
    switch(symbol)
    {
        case'~':
            return 3;
        case '?':
        case '!':
        case '+':
        case '&':
            return 2;
        case '|':
        case '-':
        case '<':
        case '=':
            return 1;
        default:
            return 0;
    }
}

void free_stack(Stack* stack)
{
    while(stack->top != NULL)
    {
        pop(stack);
    }
    free(stack);
}

state get_postfix(char* filename, char** postix )
{
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        return problem_with_opening_file;
    }
    char* line = read_line(file);
    if(line == NULL)
    {
        fclose(file);
        return empty_file;
    }
    if(strlen(line) < 1)
    {
        free(line);
        fclose(file);
        return invalid_argument;
    }
    int ch = fgetc(file);
    if(ch != EOF)
    {
        free(line);
        fclose(file);
        return not_1_string_in_file;
    }
    if(!check_brackets(line))
    {
        free(line);
        fclose(file);
        return wrong_brackets;
    }

    char* postfix = NULL;
    state result = infix_to_postfix(line, &postfix);
    if(result != well)
    {
        free(line);
        fclose(file);
        return result;
    }
    *postix = postfix;
    free(line);
    fclose(file);
    return well;
}
