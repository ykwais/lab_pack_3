#include "helper.h"


//TODO realloc for stack ✓
//TODO подумать о выводе дерева, если строки длинные ✓
//TODO стек root - несколько корней (a,b) ✓




int main(int argc, char** argv) {

    switch (check_input(argc, argv)) {
        case ic_same_file:
            printf("You've inputted the same file!\n");
            return 0;
        case ic_wrong_count_args:
            printf("wrong number of arguments!\n");
            return 0;
        case ic_empty_file:
            printf("You've entered the empty file!\n");
            return 0;
        case ic_problem_open_file:
            printf("problem with opening the file: %s\n", argv[1]);
            return 0;
        case ic_well:
            break;
    }

    FILE* in_file = NULL;
    FILE* out_file = NULL;

    if(!(in_file = fopen(argv[1], "r")))
    {
        printf("problem with opening the file: %s\n", argv[1]);
        return 0;
    }

    if(!(out_file = fopen(argv[2], "w")))
    {
        printf("problem with opening the file: %s\n", argv[2]);
        fclose(out_file);
        return 0;
    }

    int counter = 0;
    int size = 2;
    int c = 0;

    while((c = fgetc(in_file) )!= EOF)
    {
        char* strok = (char*)malloc(sizeof(char)*size);
        if(strok == NULL)
        {
            printf("problem with memory allocation!\n");
            fclose(in_file);
            fclose(out_file);
            return 0;
        }
        counter++;
        strok[counter-1] = (char)c;
        while((c = fgetc(in_file)) != '\n' && c != EOF)
        {
            counter++;
            char* tmp = NULL;
            if(counter >= size)
            {
                size *= 2;
                tmp = (char*)realloc(strok, sizeof(char)*size);
                if(tmp == NULL){
                    printf("problem with memory allocation!\n");
                    free(strok);
                    fclose(in_file);
                    fclose(out_file);
                    return 0;
                }
                else{
                    strok = tmp;
                }
            }
            strok[counter - 1] = (char)c;
        }
        strok[counter] = '\0';
        //printf("%s\n", strok);

        state st = st_well;
        node* root = get_root(strok, &st);
        switch (st){
            case mem_problem:
                printf("problem with reallocation mem!\n");
                free(strok);
                fclose(in_file);
                fclose(out_file);
                return 0;
            case st_well:
                break;
            default:
                printf("memes problem\n");
                free(strok);
                fclose(in_file);
                fclose(out_file);
                return 0;
        }
        print_tree(root, out_file, 0);
        fprintf(out_file, "==========================================================\n");


        counter = 0;
        size = 2;
        free(strok);
        free_tree(root);
    }

    fclose(in_file);
    fclose(out_file);
}



