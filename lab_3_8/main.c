#include "tmp.h"
#include <stdbool.h>
#include <ctype.h>

#define BUFFER_SIZE 512

char **split(const char *str, const char *delimiters, int *count)
{
    int token_count = 0;
    int allocated_size = 2; // начальный размер массива
    char **tokens = (char **)malloc(sizeof(char *) * allocated_size);

    if (tokens == NULL) return NULL;

    char *token = strtok((char *)str, delimiters);

    while (token != NULL)
    {
        tokens[token_count] = (char *)malloc(strlen(token) + 1);
        if (tokens[token_count] == NULL)
        {
            for (int i = 0; i < token_count; i++) { free(tokens[i]);}
            free(tokens);
            return NULL;
        }

        strcpy(tokens[token_count], token);
        token_count++;

        if (token_count >= allocated_size)
        {
            allocated_size *= 2;
            char **temp = (char **)realloc(tokens, sizeof(char *) * allocated_size);
            if (temp == NULL)
            {
                for (int i = 0; i < token_count - 1; i++) { free(tokens[i]); }
                free(tokens);
                return NULL;
            }
            tokens = temp;
        }

        token = strtok(NULL, delimiters);
    }

    *count = token_count;
    return tokens;
}


status_codes get_file_ptr(const char *file_path, FILE **file_result)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        return ERROR_WITH_FILE;
    }
    *file_result = file;
    return SUCCESS;
}

status_codes filter_and_read_line(FILE *file, char **line)
{
    char *command_line = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    int buffer_size = BUFFER_SIZE;
    int character_count = 0;

    char current_char;
    bool is_comment = false;
    bool is_big_comment = false;

    while ((current_char = fgetc(file)) != EOF && !(current_char == ';' && !is_comment && !is_big_comment))
    {
        if (buffer_size - 1 <= character_count)
        {
            char *new_command_line = (char *)realloc(command_line, sizeof(char) * buffer_size * 2);
            if (new_command_line == NULL)
            {
                free(command_line);
                return MEMORY_ALLOCATION_PROBLEM;
            }
            command_line = new_command_line;
            buffer_size *= 2;
        }

        if (!is_big_comment && current_char == '%') is_comment = true;
        if (!is_comment && current_char == '[') is_big_comment = true;
        if (current_char != '\n' && current_char != ' ' && current_char != '\t' && !is_comment && !is_big_comment)
        {
            command_line[character_count] = current_char;
            character_count++;
        }

        if (current_char == '\n' && is_comment) is_comment = false;
        if (is_big_comment && current_char == ']') is_big_comment = false;
    }

    command_line[character_count] = '\0';
    *line = command_line;

    if ((current_char = fgetc(file)) == EOF) return END_OF_FILE;

    return SUCCESS;
}


status_codes pre_build_Polynomial(char *line, int *pos, Polynomial **result, char variable)
{
    Polynomial *ptr;
    int length = strlen(line);
    int coeff = -1;
    int i = *pos;

    int sign = 1;
    if (line[i] == '+' || line[i] == '-')
    {
        sign = line[i] == '-' ? -1 : 1;
        i++;
    }

    while (i < length && isdigit(line[i]) != 0)
    {
        if (coeff == -1) coeff = 0;
        coeff *= 10;
        coeff += (line[i] - '0');
        i++;
    }
    if (coeff == -1) coeff = 1;

    coeff *= sign;
    if (i >= length || line[i] != variable)
    {
        status_codes status = create_polynomial(&ptr);
        if (status != SUCCESS)
        {
            return status;
        }
        status = add_coefficient(ptr, coeff);
        if (status != SUCCESS)
        {
            free_polinomial(ptr);
            return status;
        }
        *result = ptr;
        *pos = i;
        return SUCCESS;
    }
    i++;

    if (i >= length || line[i] != '^')
    {
        status_codes status = create_polynomial(&ptr);
        if (status != SUCCESS)
        {
            return status;
        }
        status = add_coefficient(ptr, coeff);
        if (status != SUCCESS)
        {
            free_polinomial(ptr);
            return status;
        }
        status = add_coefficient(ptr, 0);
        if (status != SUCCESS)
        {
            free_polinomial(ptr);
            return status;
        }
        *result = ptr;
        *pos = i;
        return SUCCESS;
    }
    i++;
    status_codes status = create_polynomial(&ptr);
    if (status != SUCCESS) return status;

    int power = -1;
    while (i < length && isdigit(line[i]) != 0)
    {
        if (power == -1) power += 1;
        power *= 10;
        power += (line[i] - '0');
        i++;
    }
    status = add_coefficient(ptr, coeff);
    if (status != SUCCESS)
    {
        free_polinomial(ptr);
        return status;
    }
    while (power > 0)
    {
        power--;
        status = add_coefficient(ptr, 0);
        if (status != SUCCESS) return status;
    }
    *result = ptr;
    *pos = i;
    return SUCCESS;
}


void free_array(char **input, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (input[i] != NULL) free(input[i]);
    }
    free(input);
}

status_codes build_Polynomial(char *input, int n, Polynomial **a, char variable)
{
    int l = 0;
    Polynomial *p = NULL, *current = NULL, *temper = NULL;
    status_codes status = initialization_polynomial(&current, 0);
    if (status != SUCCESS)return NOT_SUCCESS;
    int length = strlen(input);

    while (l < length)
    {
        status = pre_build_Polynomial(input, &l, &p, variable);
        if (status != SUCCESS)
        {
            free_polinomial(p);
            free_polinomial(current);
            return status;
        }
        status = summation_polynomials(current, p, &temper);
        if (status != SUCCESS)
        {
            free_polinomial(p);
            free_polinomial(current);
            free_polinomial(temper);
            return status;
        }
        free_polinomial(current);
        free_polinomial(p);
        p = NULL;
        current = temper;
    }
    *a = current;
    return SUCCESS;
}

status_codes get_long_check(char *num, int *res)
{
    char *endptr;
    long result = strtol(num, &endptr, 10);
    if (*endptr != '\0') return INVALID_INPUT;
    *res = (int)result;
    return SUCCESS;
}

status_codes validate_Polynomial(char *s, char variable)
{
    int len = strlen(s);

    if (len == 0) return INVALID_INPUT;

    int i = 0;
    bool flag = (s[0] == '+' || s[0] == '-') ? true : false;
    bool is_exponent_allowed = true;
    while (i < len)
    {
        if ((s[i] == '+' || s[i] == '-') && flag)
        {
            i++;
            flag = false;
            is_exponent_allowed = true;
        }
        else if (s[i] == variable && is_exponent_allowed)
        {
            i++;
            if (i < len && s[i] == '^')
            {
                i++;
                if (i < len && isdigit(s[i]))
                {
                    while (i < len && isdigit(s[i])){ i++; }
                }
                else return INVALID_INPUT;
            }
            is_exponent_allowed = false;
            flag = true;
        }
        else if (isdigit(s[i]) && !flag)
        {
            while (i < len && isdigit(s[i])) { i++; }
            flag = true;
        }
        else return INVALID_INPUT;
    }
    return flag ? SUCCESS : INVALID_INPUT;
}

int count_of_target(char *str, char target)
{
    int count = 0;
    while (*str)
    {
        if (*str == target) count++;
        str++;
    }
    return count;
}

status_codes validate_line(char *s)
{
    int size = strlen(s);
    if (s[size - 1] != ')' || count_of_target(s, ',') > 1) return INVALID_INPUT;
    for (int i = 0; i < size-1; i++)
    {
        if (!(isdigit(s[i]) == 0) && !(isalpha(s[i]) == 0 ) && s[i] != ',' && s[i] != '(') return INVALID_INPUT;
    }
    return SUCCESS;

}
status_codes run_command(char *line, Polynomial **results, int **Value, char variable, int ** crnt_v)
{
    int size = strlen(line);
    int cmp1 = count_of_target(line, '('),
            cmp2 = count_of_target(line, ')');

    if (strncmp(line, "Mult(", 5) != 0 && strncmp(line, "Mod(", 4) != 0 &&
        strncmp(line, "Div(", 4) != 0 && strncmp(line, "Add(", 4) != 0 &&
        strncmp(line, "Sub(", 4) != 0 && strncmp(line, "Diff(", 5) != 0 &&
        strncmp(line, "Cmps(", 5) != 0 && strncmp(line, "Eval(", 5) != 0)
    {
        return INVALID_INPUT;
    }
    if (cmp1 != cmp2 || cmp1 != 1 || validate_line(line) != SUCCESS) return INVALID_INPUT;

    char *delimiter = "()";

    int count;
    char **tokens = split(line, delimiter, &count);

    if (tokens == NULL) return NOT_SUCCESS;

    if (!(count > 1 && tokens[1] != NULL && strlen(tokens[1]) > 0))
    {
        for (int i = 0; i < count; i++)
        {
            if (tokens[i] != NULL) free(tokens[i]);
        }
        free(tokens);
        return NOT_SUCCESS;
    }
    char *polys = tokens[1];
    int n;
    char **input = split(polys, ",", &n);

    for (int i = 0; i < count; i++)
    {
        if (tokens[i] != NULL) free(tokens[i]);
    }
    free(tokens);

    for (int i = 0; i < n; i++)
    {
        status_codes statust = validate_Polynomial(input[i], variable);
        if (statust != SUCCESS)
        {
            free_array(input, n);
            return statust;
        }
    }
    if (strncmp(line, "Mult", 4) == 0 && n > 0)
    {
        Polynomial *pol1 = NULL, *pol2 = NULL;
        status_codes status = build_Polynomial(input[0], n, &pol1, variable);
        if (status != SUCCESS)
        {
            free_polinomial(pol1);
            free_polinomial(pol2);
            free_array(input, n);
            return status;
        }
        if (n == 2)
        {
            status = build_Polynomial(input[1], n, &pol2, variable);
            free_polinomial(*results);
            if (status != SUCCESS)
            {
                free_polinomial(pol1);
                free_polinomial(pol2);
                free_array(input, n);
                return status;
            }
        }
        else
        {
            pol2 = *results;
            swap(&pol1, &pol2);
        }
        status = multiply_polynomoals(pol1, pol2, results);
        free_polinomial(pol1);
        free_polinomial(pol2);
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }
    }
    else if (strncmp(line, "Add", 3) == 0 && n > 0)
    {
        Polynomial *pol1 = NULL, *pol2 = NULL;
        status_codes status = build_Polynomial(input[0], n, &pol1, variable);
        if (status != SUCCESS)
        {
            free_polinomial(pol1);
            free_polinomial(pol2);
            free_array(input, n);
            return status;
        }
        if (n == 2)
        {
            status = build_Polynomial(input[1], n, &pol2, variable);
            free_polinomial(*results);
            if (status != SUCCESS)
            {
                free_polinomial(pol1);
                free_polinomial(pol2);
                free_array(input, n);
                return status;
            }
        }
        else
        {
            pol2 = *results;
            swap(&pol1, &pol2);
        }
        status = summation_polynomials(pol1, pol2, results);
        free_polinomial(pol1);
        free_polinomial(pol2);
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }

    }
    else if (strncmp(line, "Div", 3) == 0 && n > 0)
    {
        Polynomial *pol1 = NULL, *pol2 = NULL;
        status_codes status = build_Polynomial(input[0], n, &pol1, variable);
        if (status != SUCCESS)
        {
            free_polinomial(pol1);
            free_polinomial(pol2);
            free_array(input, n);
            return status;
        }
        if (n == 2)
        {
            status = build_Polynomial(input[1], n, &pol2, variable);
            free_polinomial(*results);
            if (status != SUCCESS)
            {
                free_polinomial(pol1);
                free_polinomial(pol2);
                free_array(input, n);
                return status;
            }
        }
        else
        {
            pol2 = *results;
            swap(&pol1, &pol2);
        }
        status = div_polynomials(pol1, pol2, results);
        free_polinomial(pol1);
        free_polinomial(pol2);
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }
    }
    else if (strncmp(line, "Mod", 3) == 0 && n > 0)
    {
        Polynomial *pol1 = NULL, *pol2 = NULL;
        status_codes status = build_Polynomial(input[0], n, &pol1, variable);
        if (status != SUCCESS)
        {
            free_polinomial(pol1);
            free_polinomial(pol2);
            free_array(input, n);
            return status;
        }
        if (n == 2)
        {
            status = build_Polynomial(input[1], n, &pol2, variable);
            free_polinomial(*results);
            if (status != SUCCESS)
            {
                free_polinomial(pol1);
                free_polinomial(pol2);
                free_array(input, n);
                return status;
            }
        }
        else
        {
            pol2 = *results;
            swap(&pol1, &pol2);
        }
        Polynomial * pol3;
        status = mod_polynomials(pol1, pol2, &pol3);
        free_polinomial(pol1);
        free_polinomial(pol2);
        *results = pol3;
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }
    }
    else if (strncmp(line, "Sub", 3) == 0 && n > 0)
    {
        Polynomial *pol1 = NULL, *pol2 = NULL;
        status_codes status = build_Polynomial(input[0], n, &pol1, variable);
        if (status != SUCCESS)
        {
            free_polinomial(pol1);
            free_polinomial(pol2);
            free_array(input, n);
            return status;
        }
        if (n == 2)
        {
            free_polinomial(*results);
            status_codes status = build_Polynomial(input[1], n, &pol2, variable);
            if (status != SUCCESS)
            {
                free_polinomial(pol1);
                free_polinomial(pol2);
                free_array(input, n);
                return status;
            }
        }
        else
        {
            pol2 = *results;
            swap(&pol1, &pol2);
        }

        status = subtraction_polynomials(pol1, pol2, results);
        free_polinomial(pol1);
        free_polinomial(pol2);
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }

    }
    else if (strncmp(line, "Cmps", 4) == 0 && n > 0)
    {
        Polynomial *pol1 = NULL, *pol2 = NULL;

        status_codes status = build_Polynomial(input[0], n, &pol1, variable);
        if (status != SUCCESS)
        {
            free_polinomial(pol1);
            free_array(input, n);
            return status;
        }
        if (n == 2)
        {
            status = build_Polynomial(input[1], n, &pol2, variable);
            free_polinomial(*results);
            if (status != SUCCESS)
            {
                free_polinomial(pol2);
                free_array(input, n);
                return status;
            }
        }
        else
        {
            pol2 = *results;
            swap(&pol1, &pol2);
        }

        status = compose_polynoms(pol1, pol2, results);
        free_polinomial(pol1);
        free_polinomial(pol2);
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }
    }
    else if (strncmp(line, "Diff", 4) == 0)
    {
        Polynomial *pol;
        if (n == 1)
        {
            status_codes status = build_Polynomial(input[0], n, &pol, variable);
            free_polinomial(*results);
            if (status != SUCCESS)
            {
                free_polinomial(pol);
                free_array(input, n);
                return status;
            }
        }
        else pol = *results;

        status_codes status = diff_polynomial(pol);
        if (status != SUCCESS)
        {
            free_polinomial(pol);
            free_array(input, n);
            return status;
        }
        *results = pol;
    }
    else if (strncmp(line, "Eval", 4) == 0 && n > 0)
    {
        Polynomial *pol;
        int kk = 0;
        if (n == 2)
        {
            free_polinomial(*results);
            status_codes status = build_Polynomial(input[kk++], n, &pol, variable);
            if (status != SUCCESS)
            {
                free_polinomial(pol);
                free_array(input, n);
                return status;
            }
        }
        else pol = *results;
        long long val;
        int num;
        status_codes status = get_long_check(input[kk], &num);
        if (status != SUCCESS)
        {
            free_array(input, n);
            return status;
        }
        status = calculate_polynomial(pol, &val, num);
        if (status != SUCCESS)
        {
            free_polinomial(pol);
            free_array(input, n);
            return status;
        }
        *Value = (int *) malloc(sizeof(int));
        if (*Value == NULL) {return MEMORY_ALLOCATION_PROBLEM;}
        *crnt_v = (int *) malloc(sizeof(int));
        if (*crnt_v == NULL) {free(*Value); return MEMORY_ALLOCATION_PROBLEM;}
        *results = pol;
        **Value = val;
        **crnt_v = num;
    }
    else
    {
        free_array(input, n);
        return NOT_SUCCESS;
    }
    free_array(input, n);

    return SUCCESS;
}

status_codes task(const char * file_name, char variable)
{
    FILE *file;

    status_codes status = get_file_ptr(file_name, &file);
    if (status != SUCCESS)
    {
        fclose(file);
        return status;
    }

    Polynomial *p, *newp;
    status = initialization_polynomial(&p, 0);
    if (status != SUCCESS)
    {
        fclose(file);
        return status;
    }
    char *command;

    while ((status = filter_and_read_line(file, &command)) == SUCCESS)
    {
        int *temp = NULL, *crnt_variable = NULL;
        status = run_command(command, &p, &temp, variable, &crnt_variable);
        free(command);
        command = NULL;
        if (status != SUCCESS) break;
        if (temp == NULL)print_polynom(p, variable);
        else
        {
            printf("Polynomial eval with %c = %d is : %d\n", variable, *crnt_variable, *temp);
            free(temp);
        }

    }
    if (command != NULL) free(command);
    free_polinomial(p);
    fclose(file);
    return status;
}

void print_user_prompt(const char *executable_name)
{
    printf("To use the program, enter the following command:\n");
    printf("%s <input.txt> and variable (char)\n", executable_name);
}

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        print_status_codes(INVALID_INPUT);
        print_user_prompt(argv[0]);
        return INVALID_INPUT;
    }

    if (strlen(argv[2]) != 1) {print_status_codes(INVALID_INPUT); print_user_prompt(argv[0]); return INVALID_INPUT;}
    print_status_codes(task(argv[1], *(argv[2])));


    return 0;
}