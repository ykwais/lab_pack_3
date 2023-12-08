
#ifndef LAB_3_8_HELPER_H
#define LAB_3_8_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFF 200

typedef enum
{
    well,
    bad,
    invalid_input,
    eof,
    mem_problem,
    error_with_file,
    is_empty_file
} state;

typedef struct Variable
{
    struct Variable *next;
    int value;
} Variable;

typedef struct{
    Variable *first;
    Variable *last;
    int degree;
}Polynomial;

state filter_read_line(FILE* file, char** line);
char** split_line(const char* line, const char* separate, int* amount);
state valid_polinom(char* str, char variable);
void free_arr(char** arr, int size);
state pre_build_polinom(char *line, int *pos, Polynomial **result, char variable);
state increase_polynom(Polynomial *p, int size);
Variable* search_by_degree(const Polynomial *p, int index);
state summ_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result);
void free_elem(Variable *current);
state free_polinomial(Polynomial *p);
state build_Polynomial(char *input, int n, Polynomial **a, char variable);
state multy_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result);
state sub_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result);
state do_command(char* line, Polynomial** p, char variable, long long* value, bool* is_tem);
state calculate_polinom(const Polynomial* pol, long long* res, int num);
state diff_polinom(Polynomial* p);
state division_polinom(const Polynomial* first, const Polynomial* second, Polynomial** major, Polynomial** minor);
state div_polynom(const Polynomial *a, const Polynomial *b, Polynomial **major);
state mod_polynom(const Polynomial *a, const Polynomial *b, Polynomial **minor);
state cmp_polinom(const Polynomial* first, const Polynomial* second, Polynomial** res);
void print_polynom(Polynomial *p, char variable);
state task(const char * file_name, char variable);
void print_status_codes(state code);
state create_polynom(Polynomial** p);
state add_coefs(Polynomial* p, int coef);
state init_polynom(Polynomial** p, int value);
int count_symb(const char* str, char symb);
state check_line(char* line, char variable);
void swap_polin(Polynomial** f, Polynomial** s);
state multiply_polynomial_by_scalar(Polynomial *a, int scalar);
void print_elements(Variable *e, char variable, int degree);
#endif //LAB_3_8_HELPER_H
