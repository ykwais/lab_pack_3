#include "helper.h"

int main(int argc, char** argv) {
    if(argc != 3) {
        print_state(wrong_amount_of_arguments);
        return 0;
    }
    if(strcmp(argv[1], argv[2]) == 0){
        print_state(same_file);
        return 0;
    }
    HashTable* ht = create_hashtable(HASHSIZE);
    if(ht == NULL){
        print_state(meme_problem);
        return 0;
    }

    state st = file_work(argv[1], argv[2], &ht);

    remove_hashtable(ht);

    print_state(st);
    return 0;

}





