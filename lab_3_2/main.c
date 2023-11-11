#include "helper.h"

int main() {
    int troubles = 0;
    vector a,b,c,d,e,f,z;

    make_vector( &a, &troubles, 3, 1.0, 2.0, 3.0);
    make_vector( &b, &troubles, 3, -24.0, 0.0, 1.0);
    make_vector( &c, &troubles, 3, 24.0, 3.0, 3.0);
    make_vector( &d, &troubles, 3, 15.0, -17.0, 23.0);
    make_vector( &e, &troubles, 3, 6.0, 9.0, 24.0);
    make_vector(&f, &troubles, 3, 15.0, 17.0, 23.0);
    make_vector(&z, &troubles, 3, -15.0, -23.0, -17.0);


    if(troubles != 0)
    {
        printf("problem with memory allocation!\n");
        return 0;
    }

    vector** result_arr = NULL;
    int size_norm_1 = 0;
    int size_norm_2 = 0;
    int size_norm_3 = 0;

    switch(get_the_longest(&result_arr, &size_norm_1, &size_norm_2, &size_norm_3, 3, 3, &norm_1, &norm_2, &norm_3, 7, a,b,c,d,e,f,z)){
        case mem_problem:
            printf("mem problem\n");
            delete_vectors(7,a,b,c,d,e,f,z);

            return 0;
        case invalid:
            printf("invalid\n");
            delete_vectors(7,a,b,c,d,e,f,z);

            return 0;
        case well:
            printf("ok\n");

            break;
    }




    print_results(result_arr, size_norm_1, size_norm_2, size_norm_3);



    delete_vectors(7,a,b,c,d,e,f,z);

    free(result_arr);

}


















