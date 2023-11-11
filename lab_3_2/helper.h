
#ifndef LAB_3_2_HELPER_H
#define LAB_3_2_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>


typedef enum status_code{
    well,
    mem_problem,
    invalid
} state;

typedef struct{
    double* coordinates;
    int size;
} vector;


state make_vector(vector* vec, int* counter, int dim, ...);
void print_vector(vector vec);
double norm_1(vector vec, int dim);
state norm_2(vector vec, int dim, int p, double* result );
state norm_3(vector vec, int dim, double* result);
state get_the_longest(vector*** result_arr, int* first, int* second, int* third, int dim, int p, double(*norm_1)(vector vec, int dim), state(*norm_2)(vector vec, int dim, int p, double* result), state(*norm_3)(vector vec, int dim, double* result), int count_vecs, ...);
void print_results(vector** res, int count_1, int count_2, int count_3);
void delete_vectors( int count_vectors, ...);
void delete_vector(vector vec);

#endif //LAB_3_2_HELPER_H
