#include "helper.h"

list main_list;
undo_stack stack;

void signal_handler(int x){//for ctrl+C
    if(x == SIGINT){
        free_list(&main_list);
        delete_stack(&stack);
        exit(0);
    }

}

int main(int argc, char** argv){

    signal(SIGINT, signal_handler);

    if(argc != 2){
        printf("wrong number of arguments!\n");
        return 0;
    }


    stack.size = 0;
    stack.buf = 0;
    stack.nodes = NULL;
    main_list.in_head = NULL;
    switch(parsing_input_data(argv[1], &main_list)){
        case is_exist_liver:
            printf("you've entered the same livers!\n");
            free_list(&main_list);
            return 0;
        case is_mem_problem:
            printf("there are memory problems!\n");
            free_list(&main_list);
            return 0;
        case is_invalid_string:
            printf("you've inputted the wrong string of data!\n");
            free_list(&main_list);
            return 0;
        case is_empty_file:
            printf("you've inputted the empty file!\n");
            free_list(&main_list);
            return 0;
        case is_problem_open_file:
            printf("problem with opening the file: %s\n", argv[1]);
            free_list(&main_list);
            return 0;
        case is_empty_str:
            printf("there is the empty string in file. It should be fixed!\n");
            free_list(&main_list);
            return 0;
        case is_wrong_amount_lex_in_str:
            printf("you've inputted the wrong amount of tokens in string!\n");
            free_list(&main_list);
            return 0;
        case is_problem_with_male:
            printf("you've inputted the wrong male of liver!\n");
            free_list(&main_list);
            return 0;
        case is_problem_with_token:
            printf("problem with token in file!\n");
            free_list(&main_list);
            return 0;
        case is_well:
            printf("data were collected\n");
            break;
    }

    //print_list(&main_list, "stdout");
    input_state status = interactive(&main_list, &stack);
    if(status == is_mem_problem){
        printf("memory problem!\n");
        free_list(&main_list);
        delete_stack(&stack);
        return 0;
    }
    free_list(&main_list);
    delete_stack(&stack);
}
