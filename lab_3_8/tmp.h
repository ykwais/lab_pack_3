//
// Created by fedor on 07.12.2023.
//

#ifndef LAB_3_8_TMP_H
#define LAB_3_8_TMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef enum
{
    SUCCESS,
    NOT_SUCCESS,
    INVALID_INPUT,
    END_OF_FILE,
    MEMORY_ALLOCATION_PROBLEM,
    ERROR_WITH_FILE
} status_codes;

void print_status_codes(status_codes code)
{
    switch (code)
    {
        case SUCCESS:
            printf("Success\n");
            break;
        case NOT_SUCCESS:
            printf("Not success\n");
            break;
        case INVALID_INPUT:
            printf("Invalid input\n");
            break;
        case END_OF_FILE:
            printf("End of file\n");
            break;
        case MEMORY_ALLOCATION_PROBLEM:
            printf("Memory allocation problem\n");
            break;
        case ERROR_WITH_FILE:
            printf("Error with file\n");
            break;
        default:
            printf("Unknown status code\n");
            break;
    }
}



typedef struct Variable
{
    struct Variable *next;
    int value;
} Variable;

typedef struct
{
    Variable *first;
    Variable *last;
    int degree;
} Polynomial;

void swap(Polynomial **a, Polynomial **b)
{
    Polynomial **temp = a;
    a = b;
    b = temp;
}

status_codes create_polynomial(Polynomial **result)
{
    Polynomial *p = (Polynomial *) malloc(sizeof(Polynomial));
    if (p == NULL) {return MEMORY_ALLOCATION_PROBLEM; }

    p->degree = 0;
    p->first = (Variable *) malloc(sizeof(Variable));
    if (p->first == NULL) {free(p); return MEMORY_ALLOCATION_PROBLEM; }

    p->last = p->first;
    p->first->next = NULL;

    *result = p;
    return SUCCESS;
}


Variable* search_by_degree(const Polynomial *p, int index)
{
    Variable *ptr = p->first;
    int i = 0;
    while (i < index)
    {
        ptr = ptr->next;
        i++;
    }
    return ptr;
}

void free_elements(Variable *current)
{
    if (current == NULL) { return; }
    if (current->next != NULL)
    {
        free_elements(current->next);
    }
    free(current);

}

status_codes free_polinomial(Polynomial *p)
{
    if (p == NULL) {return INVALID_INPUT; }
    if (p->first != NULL) { free_elements(p->first); }
    free(p);
    p = NULL;
    return SUCCESS;
}


void print_elements(Variable *e, char variable, int degree)
{
    if (e == NULL) {return; }
    printf("%d*%c^%d", e->value, variable, degree);
    if (e->next != NULL)
    {
        printf(" + ");
        print_elements(e->next, variable, degree - 1);
    }
}

void print_polynom(Polynomial *p, char variable)
{
    if (p == NULL) { return; }
    printf("Degree of current polynomial is %d : ", p->degree - 1);
    print_elements(p->first, variable, p->degree - 1);
    printf("\n");
}


status_codes calculate_polynomial(const Polynomial *p, long long *res, int variable)
{
    if (p == NULL) {return INVALID_INPUT; }
    *res = 0;
    Variable *ptr = p->first;
    for (int i = 0; i < p->degree; i++)
    {
        if (ptr == NULL) {  break; } //Gorner
        *res = (*res) * variable;
        *res = (*res) + ptr->value;
        ptr = ptr->next;

    }
    return SUCCESS;
}


status_codes diff_polynomial(Polynomial *p)
{
    if (p == NULL) {return INVALID_INPUT; }
    if (p->degree - 1 == 0)
    {
        p->first->value = 0;
        return SUCCESS;
    }

    Variable *ptr = p->first;
    for (int i = 0; i < p->degree - 1; i++)
    {
        ptr->value = ptr->value * ((p->degree) - i - 1);
        ptr = ptr->next;
    }

    Variable *e = search_by_degree(p, p->degree - 2);
    free_elements(ptr);
    (p->degree) = (p->degree) - 1;
    e->next = NULL;

    return SUCCESS;
}


status_codes add_coefficient(Polynomial *p, int k)
{
    if (p->degree == 0){
        p->degree = p->degree + 1;
        p->first->value = k;
        return SUCCESS;
    }
    Variable *ptr = p->last;

    ptr->next = (Variable *) malloc(sizeof(Variable));
    if (ptr->next == NULL)
    {
        return MEMORY_ALLOCATION_PROBLEM;
    }

    ptr->next->next = NULL;
    p->last = ptr->next;
    p->last->value = k;

    p->degree = (p->degree) + 1;

    return SUCCESS;
}

status_codes initialization_polynomial(Polynomial **p, int value)
{
    status_codes status = create_polynomial(p);
    if (status != SUCCESS) return status;
    status = add_coefficient(*p, value);
    if (status != SUCCESS) free_polinomial(*p);
    return status;
}

status_codes increase_polynomial (Polynomial *p, int size)
{
    if (size < p->degree ) {return INVALID_INPUT; }
    while (size > p->degree)
    {
        status_codes status = add_coefficient(p, 0);
        if (status != SUCCESS) {return status;}
    }
    return SUCCESS;
}

status_codes subtraction_polynomials(const Polynomial *first, const Polynomial *second, Polynomial **result)
{
    Polynomial *p;
    status_codes status = create_polynomial(&p);
    if (status != SUCCESS) return status;
    int size = (first->degree > second->degree) ? first->degree : second->degree;

    status = increase_polynomial(p, size);
    if (status != SUCCESS) return status;

    for (int i = first->degree; i > 0; i--)
    {
        Variable *a = search_by_degree(p, size - i);
        Variable *b = search_by_degree(first, first->degree - i);
        a->value = b->value;
    }

    for (int i = second->degree; i > 0; i--)
    {
        Variable *a = search_by_degree(p, size - i);
        Variable *b = search_by_degree(second, second->degree - i);
        a->value =  a->value - b->value;
    }

    *result = p;
    return SUCCESS;
}


status_codes summation_polynomials(const Polynomial *first, const Polynomial *second, Polynomial **result)
{
    Polynomial *p;
    status_codes status = create_polynomial(&p);
    if (status != SUCCESS) return status;
    int size = (first->degree > second->degree) ? first->degree : second->degree;

    status = increase_polynomial(p, size);
    if (status != SUCCESS) return status;

    for (int i = first->degree; i > 0; i--)
    {
        Variable *a = search_by_degree(p, size - i);
        Variable *b = search_by_degree(first, first->degree - i);
        a->value = b->value;
    }

    for (int i = second->degree; i > 0; i--)
    {
        Variable *a = search_by_degree(p, size - i);
        Variable *b = search_by_degree(second, second->degree - i);
        a->value =  a->value + b->value;
    }

    *result = p;
    return SUCCESS;
}


status_codes multiply_polynomoals(const Polynomial *a, const Polynomial *b, Polynomial **result) {
    Polynomial *p;
    status_codes status = create_polynomial(&p);
    if (status != SUCCESS) {
        return status;
    }

    int size = a->degree + b->degree - 1; // Используем -1, так как степень произведения полиномов равна сумме степеней, а не их произведению.

    status = increase_polynomial(p, size);
    if (status != SUCCESS) {
        return status;
    }

    for (int i = 0; i < a->degree; i++) {
        Variable *_i = search_by_degree(a, i);
        for (int j = 0; j < b->degree; j++) {
            Variable *_j = search_by_degree(b, j);
            Variable *_ij = search_by_degree(p, i + j);
            _ij->value += (_i->value * _j->value);
        }
    }

    *result = p;
    return SUCCESS;
}



status_codes devide_polynomials(const Polynomial *a, const Polynomial *b, Polynomial **main_part, Polynomial **remainder_part)
{
    Polynomial *main, *remainder;

    status_codes status = create_polynomial(&main);
    if (status != SUCCESS) { return status; }

    status = create_polynomial(&remainder);
    if (status != SUCCESS) { free_polinomial(main); return status; }

    int size = a->degree;

    status = increase_polynomial(remainder, size);
    if (status != SUCCESS) { free_polinomial(remainder); free_polinomial(main); return status; }

    status = increase_polynomial(main, size);
    if (status != SUCCESS) { free_polinomial(remainder); free_polinomial(main); return status; }


    if (a->degree < b->degree) {return SUCCESS; }

    Variable *ptr = a->first, *ptr2 = remainder->first;

    for (int i = 0; i < a->degree; i++)
    {
        ptr2->value = ptr->value;
        ptr = ptr->next;
        ptr2 = ptr2->next;
    }
    ptr = remainder->first;
    ptr2 = main->first;


    for (int i = 0; i <= (a->degree - b->degree); i++)
    {
        if (b->first->value == 0)
        {
            free_polinomial(remainder); free_polinomial(main);
            return INVALID_INPUT;
        }
        int coef = (int)(ptr->value / b->first->value);
        ptr2->value= coef;
        Variable *_j = b->first;
        for (int j = 0; j < b->degree; j++)
        {
            Variable *_ij = search_by_degree(remainder, i+j);
            _ij->value = _ij->value - coef * _j->value;
            _j = _j->next;
        }
        ptr = ptr->next;
        ptr2 = ptr2->next;
    }

    ptr = remainder->first;
    Variable *endpoint = remainder->first;
    int degree = 0;
    for (int i = 0; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->value != 0)
        {
            endpoint = ptr;
            degree = i + 1;
        }
        i++;
    }
    remainder->degree = degree;
    free_elements(endpoint->next);
    endpoint->next = NULL;

    if (remainder->degree == 0)
    {
        free_polinomial(remainder);
        status = create_polynomial(&remainder);
        if (status != SUCCESS) { free_polinomial(main); return status; }

        status = increase_polynomial(remainder, 1);
        if (status != SUCCESS) { free_polinomial(main); return status; }
        remainder->degree = 1;
    }

    degree = 0;
    ptr = main->first;
    endpoint = main->first;

    for (int i = 0; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->value != 0)
        {
            endpoint = ptr;
            degree = i + 1;
        }
        i++;
    }

    main->degree = degree;
    free_elements(endpoint->next);

    endpoint->next = NULL;

    if (main->degree == 0)
    {
        free_polinomial(main);
        status = create_polynomial(&main);
        if (status != SUCCESS) { free_polinomial(remainder); return status; }

        status = increase_polynomial(main, 1);
        if (status != SUCCESS) { free_polinomial(remainder); return status; }
        main->degree = 1;
    }

    *main_part = main;
    *remainder_part = remainder;
    return SUCCESS;
}

status_codes div_polynomials(const Polynomial *a, const Polynomial *b, Polynomial **main_part)
{
    Polynomial *temp = NULL;
    status_codes status = devide_polynomials(a, b, main_part, &temp);
    if (status != SUCCESS) { return status ;}
    free_polinomial(temp);
    return SUCCESS;
}

status_codes mod_polynomials(const Polynomial *a, const Polynomial *b, Polynomial **reminder_part)
{
    Polynomial *temp = NULL;
    status_codes status = devide_polynomials(a, b, &temp, reminder_part);
    if (status != SUCCESS) { return status ;}
    free_polinomial(temp);
    return SUCCESS;
}

status_codes multiply_polynomial_by_scalar(Polynomial *a, int scalar)
{
    Variable *ptrA = a->first;
    for (int i = 0; i < a->degree; i++)
    {
        ptrA->value *= scalar;
        ptrA = ptrA->next;
    }

    return SUCCESS;
}




status_codes compose_polynoms(const Polynomial *A, const Polynomial *B, Polynomial **resulting)
{
    Polynomial *result;
    status_codes status;

    status = create_polynomial(&result);
    if (status != SUCCESS) {
        return status;
    }

    Variable *ptrA = A->first;
    int degree = A->degree - 1;

    while (ptrA != NULL)
    {
        Polynomial *term = NULL;
        status = initialization_polynomial(&term, 1);
        if (status != SUCCESS)
        {
            free_polinomial(result);
            return status;
        }

        while((degree--) > 0)
        {
            status = multiply_polynomoals(term, B, &term);
            if (status != SUCCESS) {
                free_polinomial(result);
                free_polinomial(term);
                return status;
            }
        }

        multiply_polynomial_by_scalar(term, ptrA->value);
        status = summation_polynomials(result, term, &result);
        if (status != SUCCESS)
        {
            free_polinomial(result);
            free_polinomial(term);
            return status;
        }

        free_polinomial(term);
        ptrA = ptrA->next;
    }

    *resulting = result;
    return SUCCESS;
}



#endif //LAB_3_8_TMP_H
