#include "helper.h"

void print_state(state st, char* filename, FILE* file, const int* count, char* infix, char* postfix, const long long int* result)
{
    switch(st)
    {
        case invalid_argument:
            fprintf(file, "Inavalid amount of arguments!\n");
            break;
        case well:
            fprintf(file, "Well expression:\n");
            fprintf(file, "File: %s\n", filename);
            fprintf(file, "infix: %s\n", infix);
            fprintf(file, "postfix: %s\n", postfix);
            fprintf(file, "value: %lld\n\n", *result);
            break;
        case empty_file:
            fprintf(file,"Empty file or memory allocation problem: %s\n", filename);
            break;
        case memory_problem:
            fprintf(file,"Memory problem\n");
            break;
        case invalid_symbol:
            fprintf(file, "Error detected in file: %s\n", filename);
            fprintf(file,"Type: Invalid symbol\n");
            fprintf(file, "Number of expression: %d\n", *count);
            fprintf(file, "Expression: %s\n\n", infix);
            break;
        case problem_with_opening_file:
            fprintf(file, "Problem with opening file: %s\n", filename);
            break;
        case wrong_brackets:
            fprintf(file, "Error detected in file: %s\n", filename);
            fprintf(file,"Type: Wrong brackets\n");
            fprintf(file, "Number of expression: %d\n", *count);
            fprintf(file, "Expression: %s\n\n", infix);
            break;
        case not_1_string_in_file:
            fprintf(file,"there is not 1 string in file, check input file!\n");
            break;
        case wrong_operation:
            fprintf(file,"Wrong operation in postfix\n");
            break;
        case overflow:
            fprintf(file, "Error detected in file: %s\n", filename);
            fprintf(file, "Type: Overflow\n");
            fprintf(file, "Number of expression: %d\n", *count);
            fprintf(file, "Expression: %s\n\n", infix);
            break;
        case division_by_zero:
            fprintf(file, "Error detected in file: %s\n", filename);
            fprintf(file, "Type: Division by zero\n");
            fprintf(file, "Number of expression: %d\n", *count);
            fprintf(file, "Expression: %s\n\n", infix);
            break;
        case invalid_value:
            fprintf(file, "Error detected in file: %s\n", filename);
            fprintf(file, "Type: Invalid value\n");
            fprintf(file, "Number of expression: %d\n", *count);
            fprintf(file, "Expression: %s\n\n", infix);
            break;

        default:
            fprintf(file, "Unknown state\n");
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

    bool double_operation = false;

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
            double_operation = false;
            while(isdigit(*ptr))
            {
                st = add_symbol_in_array_char(&result, &size, &buf, *ptr);
                if(st != well)
                {
                    free_stack(stack);
                    return st;
                }
                ptr++;
            }
            st = add_symbol_in_array_char(&result, &size, &buf, ' ');
            if(st != well)
            {
                free_stack(stack);
                return st;
            }
            continue;

        }
        else if (*ptr == '(') {
            double_operation = false;
            st = push(stack, *ptr);
            if (st != well) {
                free(result);
                free_stack(stack);
                return st;
            }
        } else if (*ptr == ')') {
            double_operation = false;
            while (stack->top != NULL && stack->top->symbol != '(') {

                st = add_symbol_in_array_char(&result, &size, &buf, stack->top->symbol);
                if(st != well)
                {
                    free_stack(stack);
                    return st;
                }
                st = add_symbol_in_array_char(&result, &size, &buf, ' ');
                if(st != well)
                {
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
        else if((*ptr == '+' || *ptr == '-' || *ptr == '/' || *ptr == '%' || *ptr == '*' || *ptr == '^') && !double_operation) {
            double_operation = true;
            while (stack->top != NULL && priority_of_symbol(stack->top->symbol) >= priority_of_symbol(*ptr))
            {
                st = add_symbol_in_array_char(&result, &size, &buf, stack->top->symbol);
                if(st != well)
                {
                    free_stack(stack);
                    return st;
                }
                st = add_symbol_in_array_char(&result, &size, &buf, ' ');
                if(st != well)
                {
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
            free_stack(stack);
            return st;
        }
        st = add_symbol_in_array_char(&result, &size, &buf, ' ');
        if(st != well)
        {
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
        return invalid_symbol;
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
    int len = (int)strlen(filename);
    int size = 0;
    int buf = len + 6;
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
                    if(len1 + len2 > 4 || (first > 899  && second > 6))
                    {
                        free_val_stack(stack);
                        return overflow;
                    }
                    num = bpow(first, second);
                    if(num == 0)
                    {
                        free_val_stack(stack);
                        return overflow;
                    }
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
    int size = 0;
    Val_node* tmp = stack->bottom;
    while(tmp != NULL)
    {
        size++;
        tmp = tmp->next;
    }
    if(size != 1)
    {
        free_val_stack(stack);
        return invalid_symbol;
    }


    *result = stack->top->value;

    free_val_stack(stack);
    return well;
}


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