#include "helper.h"

out_list main_list;

void signal_handler(int x){//for ctrl+C
    if(x == SIGINT){
        free_list_global(&main_list);
        exit(0);
    }

}


int main(int argc, char** argv) {

    signal(SIGINT, signal_handler);

    if(argc < 2){
        printf("too few arguments\n");
    }

    //out_list main_list;
    main_list.out_head = NULL;

    for(int i = 1; i < argc; ++i){

        switch(parsing_input_data(argv[i], &main_list)){
            case ic_mem_problem:
                printf("meme problem\n");
                free_list_global(&main_list);
                return 0;
            case ic_problem_open_file:
                printf("problem with open file: %s\n", argv[i]);
                free_list_global(&main_list);
                return 0;
            case ic_invalid_string:
                break;
            case ic_empty_file:
                printf("you've inputted the empty file: %s\n", argv[i]);
                free_list_global(&main_list);
                return 0;
            case ic_empty_str:
                printf("there is empty str in file!\n");
                free_list_global(&main_list);
                return 0;
            case ic_same_coordination:
                printf("you've entered the same coordination in different files! or NULL in function\n");
                free_list_global(&main_list);
                return 0;
            case ic_well:
                break;
        }

    }
    if(check_all(&main_list) == cb_invalid){
        printf("incorrect data\n");
        free_list_global(&main_list);
        return 0;
    }
    switch(check_time_bus(&main_list)){
        case same_time_arriving_and_departure:
            printf("you've inputted the same arrive and departure time in one bus\n");
            free_list_global(&main_list);
            return 0;
        case same_time_all:
            printf("you've inputted the same time of arriving and departure for one bus\n");
            free_list_global(&main_list);
            return 0;
        case same_time_departure:
            printf("you've inputted the same time of departure\n");
            free_list_global(&main_list);
            return 0;
        case same_time_arrive:
            printf("you've inputted the same time of arriving\n");
            free_list_global(&main_list);
            return 0;
        case departure_less_arrive:
            printf("you've inputted departure time less arriving time for one bus\n");
            free_list_global(&main_list);
            return 0;
        case departure_more_than_arriving:
            printf("you've inputted departure time more or equal than some other arriving time\n");
            free_list_global(&main_list);
            return 0;
        case well_time:
            break;
    }
    switch (check_coordination(&main_list)) {
        case cb_invalid:
            printf("you've entered the same coordinates for two consecutive stops of the same bus\n");
            free_list_global(&main_list);
            return 0;
        case cb_well:
            break;
    }
    //print_all(&main_list);
    interactive(&main_list);
    free_list_global(&main_list);
}
