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
void print_vector(vector* vec);
double norm_1(vector vec, int dim);
state norm_2(vector vec, int dim, int p, double* result );
state norm_3(vector vec, int dim, double* result);
state get_the_longest(vector*** result_arr, int* first, int* second, int* third, int dim, int p, double(*norm_1)(vector vec, int dim), state(*norm_2)(vector vec, int dim, int p, double* result), state(*norm_3)(vector vec, int dim, double* result), int count_vecs, ...);
void print_results(vector** res, int count_1, int count_2, int count_3);
void delete_vectors( int count_vectors, ...);
void delete_vector(vector vec);

int main() {
    int troubles = 0;
    vector a,b,c,d,e,f;

    make_vector( &a, &troubles, 3, 1.0, 2.0, 3.0);
    make_vector( &b, &troubles, 3, -24.0, 0.0, 1.0);
    make_vector(&f, &troubles, 3, -15.0, 17.0, 23.0);
    make_vector( &c, &troubles, 3, 24.0, 3.0, 3.0);
    make_vector( &d, &troubles, 3, 15.0, -17.0, 23.0);
    make_vector( &e, &troubles, 3, 6.0, 9.0, 24.0);


    if(troubles != 0)
    {
        printf("problem with memory allocation!\n");
        return 0;
    }

    vector** result_arr = NULL;
    int size_norm_1 = 0;
    int size_norm_2 = 0;
    int size_norm_3 = 0;

    get_the_longest(&result_arr, &size_norm_1, &size_norm_2, &size_norm_3, 3, 3, &norm_1, &norm_2, &norm_3, 6, a,b,c,d,e,f);

    print_results(result_arr, size_norm_1, size_norm_2, size_norm_3);



    delete_vectors(6,a,b,c,d,e,f);
    free(result_arr[0]);
    free(result_arr[1]);
    free(result_arr[2]);
    free(result_arr);


}

void print_results(vector** res, int count_1, int count_2, int count_3)
{
    printf("norm 1:\n\n");
    for (int i = 0; i < count_1; ++i)
    {
        print_vector(&res[0][i]);
        printf("\n");
    }
    printf("norm 2:\n\n");
    for (int i = 0; i < count_2; ++i)
    {
        print_vector(&res[1][i]);
        printf("\n");
    }
    printf("norm 3:\n\n");
    for (int i = 0; i < count_3; ++i)
    {
        print_vector(&res[2][i]);
        printf("\n");
    }
}

void print_vector(vector* vec)
{
    printf("size: %d; coord: ", vec->size);
    for(int i = 0; i < vec->size; ++i)
    {
        printf("%f ", vec -> coordinates[i]);
    }
    printf("\n");

}

void delete_vector(vector vec)
{
    free(vec.coordinates);
}

void delete_vectors(int count_vectors, ...)
{
    va_list ptr;
    va_start(ptr, count_vectors);
    for (int i = 0; i < count_vectors; ++i)
    {
        delete_vector(va_arg(ptr, vector));
    }
    va_end(ptr);
}

state make_vector(vector* vec, int* counter, int dim,...)
{
    vec -> coordinates = (double*)malloc(sizeof(double)*dim);
    if(vec -> coordinates == NULL)
    {
        (*counter)++;
        return mem_problem;
    }
    vec -> size = dim;
    va_list ptr;
    va_start(ptr, dim);
    for(int i = 0; i < dim; ++i)
    {
        vec -> coordinates[i] = va_arg(ptr, double);
    }
    va_end(ptr);
    return well;
}



double norm_1(vector vec, int dim)
{
    double result = 0;
    for (int i = 0; i < dim; ++i)
    {
        result = fmax(result, fabs(vec.coordinates[i]));
    }
    return result;
}

state norm_2(vector vec, int dim, int p, double* result )
{
    if(p < 1){
        return invalid;
    }
    double summ = (double)0;
    for(int i = 0; i < dim; ++i){
        summ += pow(fabs(vec.coordinates[i]), p);
    }
    *result = pow(summ, (1.0/p));
    return well;
}

state norm_3(vector vec, int dim, double* result)
{
    double** matrix = (double**)malloc(sizeof(double*)*dim);
    if(matrix == NULL){
        return mem_problem;
    }
    for(int i = 0; i < dim; ++i)
    {
        matrix[i] = (double*)malloc(sizeof(double)*dim);
        if(matrix[i] == NULL){
            for(int w = 0; w < i; w++){
                free(matrix[w]);
            }
            free(matrix);
            return mem_problem;
        }
        for(int j = 0; j < dim; ++j)
        {
            matrix[i][j] = (i == j) ? 1:0;//E-матрица - положительно определенная матрица
        }
    }


    vector collection;                                               //Ex = x!!!
    collection.size = dim;
    collection.coordinates = (double*)malloc(sizeof(double)*dim);
    if(collection.coordinates == NULL){
        for(int i = 0; i < dim; ++i){
            free(matrix[i]);
        }
        free(matrix);
        return mem_problem;
    }
    for(int i = 0; i < dim; ++i){
        collection.coordinates[i] = (double)0;
    }

    for(int i = 0; i < dim; ++i) {

        for(int j = 0; j < dim; j++)
        {
            collection.coordinates[i] += vec.coordinates[j] * matrix[i][j];//Ex = x!!!
        }
    }

    double answer = 0l;

    for(int i = 0; i < dim; ++i)
    {
        answer += collection.coordinates[i] * vec.coordinates[i];
    }

    *result = sqrt(answer);

    for(int i = 0; i < dim; ++i){
        free(matrix[i]);
    }
    free(matrix);
    free(collection.coordinates);

    return well;
}

state get_the_longest(vector*** result_arr, int* first, int* second, int* third, int dim, int p, double(*norm_1)(vector vec, int dim), state(*norm_2)(vector vec, int dim, int p, double* result), state(*norm_3)(vector vec, int dim, double* result), int count_vecs, ...){
    *result_arr = (vector**)malloc(sizeof(vector*)*3);
    if(*result_arr == NULL){
        return mem_problem;
    }

    double arr_norm_1_value[count_vecs];
    double arr_norm_2_value[count_vecs];
    double arr_norm_3_value[count_vecs];
    vector vecs[count_vecs];

    va_list ptr;
    va_start(ptr, count_vecs);

    for(int i = 0; i < count_vecs; ++i){

        vector current = va_arg(ptr, vector);
        vecs[i] = current;

        arr_norm_1_value[i] = norm_1(current, dim);

        double tmp = -10;
        switch(norm_2(current, dim, p, &tmp)){
            case invalid:
                free(*result_arr);
                va_end(ptr);
                return invalid;
            case mem_problem:
                break;
            case well:
                arr_norm_2_value[i] = tmp;
                break;
        }
        switch(norm_3(current, dim, &tmp)){
            case invalid:
                break;
            case mem_problem:
                free(*result_arr);
                va_end(ptr);
                return mem_problem;
            case well:
                arr_norm_3_value[i] = tmp;
        }
    }
    va_end(ptr);

    double max_norm_1 = -1;
    double max_norm_2 = -1;
    double max_norm_3 = -1;

    for(int i = 0; i < count_vecs; ++i)
    {
        max_norm_1 = fmax(max_norm_1, arr_norm_1_value[i]);
        max_norm_2 = fmax(max_norm_2, arr_norm_2_value[i]);
        max_norm_3 = fmax(max_norm_3, arr_norm_3_value[i]);
    }

    printf("norm_1 %f\n", max_norm_1);
    printf("norm_2 %f\n", max_norm_2);
    printf("norm_3 %f\n", max_norm_3);


    int count_max_1 = 1;
    int count_max_2 = 1;
    int count_max_3 = 1;
    int count_1 = 0;
    int count_2 = 0;
    int count_3 = 0;

    (*result_arr)[0] = (vector*)malloc(sizeof(vector)*count_max_1);
    if((*result_arr)[0] == NULL){
        free(*result_arr);
        return mem_problem;
    }

    (*result_arr)[1] = (vector*)malloc(sizeof(vector)*count_max_2);
    if((*result_arr)[1] == NULL){
        free((*result_arr)[0]);
        free(*result_arr);
        return mem_problem;
    }

    (*result_arr)[2] = (vector*)malloc(sizeof(vector)*count_max_3);
    if((*result_arr)[2] == NULL){
        free((*result_arr)[0]);
        free((*result_arr)[1]);
        free(*result_arr);
        return mem_problem;
    }


    for(int i = 0; i < count_vecs; ++i)
    {
        if(max_norm_1 == arr_norm_1_value[i]){
            if(count_1 >= count_max_1){
                count_max_1 *= 2;
                vector* tmp = (vector*) realloc((*result_arr)[0], sizeof(vector)*count_max_1);
                if(tmp == NULL){
                    free((*result_arr)[1]);
                    free((*result_arr)[2]);
                    free(*result_arr);
                    return mem_problem;
                }
                (*result_arr)[0] = tmp;
            }
            (*result_arr)[0][count_1] = vecs[i];
            count_1++;
        }
        if(max_norm_2 == arr_norm_2_value[i]){
            if(count_2 >= count_max_2){
                count_max_2 *= 2;
                vector* tmp = (vector*) realloc((*result_arr)[1], sizeof(vector)*count_max_2);
                if(tmp == NULL){
                    free((*result_arr)[0]);
                    free((*result_arr)[2]);
                    free(*result_arr);
                    return mem_problem;
                }
                (*result_arr)[1] = tmp;
            }
            (*result_arr)[1][count_2] = vecs[i];
            count_2++;
        }
        if(max_norm_3 == arr_norm_3_value[i]){
            if(count_3 >= count_max_1){
                count_max_3 *= 2;
                vector* tmp = (vector*) realloc((*result_arr)[0], sizeof(vector)*count_max_3);
                if(tmp == NULL){
                    free((*result_arr)[1]);
                    free((*result_arr)[0]);
                    free(*result_arr);
                    return mem_problem;
                }
                (*result_arr)[2] = tmp;
            }
            (*result_arr)[2][count_3] = vecs[i];
            count_3++;
        }
    }
    *first = count_1;
    *second = count_2;
    *third = count_3;

    return well;

}
















