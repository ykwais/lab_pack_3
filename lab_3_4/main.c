#include "helper.h"


int main() {
    state st = well;

    string* test = create_string("", &st);
    if(st == well){
        printf("well: %s\n", test->arg);
        printf("size: %d\n",test->size);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        return 0;
    }

    string* test_2 = create_string("235", &st);
    if(st == well){
        printf("well: %s\n", test_2->arg);
        printf("size: %d\n",test_2->size);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }

    int comp = equal(test, test_2, &st);
    if(st == well){
        printf("comp: %d\n", comp);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }

    int comp_2 = order_relation_string(test, test_2, &st);
    if(st == well){
        printf("comp: %d\n", comp_2);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    }

    printf("test concat:\n");
    printf("\nfrom: %s\n", test->arg);
    printf("to: %s\n", test_2->arg);

    switch(cnt_string(test, test_2)){
        case invalid:
            printf("invalid input\n");
            delete_string(test);
            delete_string(test_2);
            return 0;
        case meme_problem:
            printf("mememe problem\n");
            delete_string(test);
            delete_string(test_2);
            return 0;
        case well:


            printf("\nfrom: %s\n", test->arg);
            printf("to: %s\n", test_2->arg);
            break;


    }


    switch(copy_to_exist(test, test_2)){
    case invalid:
        printf("invalid input\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    case meme_problem:
        printf("mememe problem\n");
        delete_string(test);
        delete_string(test_2);
        return 0;
    case well:
        printf("\nfrom: %s\n", test->arg);
        printf("to: %s\n", test_2->arg);
        break;


    }

    string* test_3 = copy_to_new_string(test, &st);
    if(st == well){
        printf("test_3: %s\n", test_3->arg);
        printf("size test_3: %d\n", test_3->size);
    }
    if(st == invalid){
        printf("invalid\n");
        delete_string(test);
        delete_string(test_2);
        delete_string(test_3);
        return 0;
    }
    if(st == meme_problem){
        printf("meme problem\n");
        delete_string(test);
        delete_string(test_2);
        delete_string(test_3);

        return 0;
    }

    delete_string(test);
    delete_string(test_2);
    delete_string(test_3);
}

