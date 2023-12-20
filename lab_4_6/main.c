#include "helper.h"


int main(int argc, char** argv) {
    if(argc != 2)
    {
        print_state(invalid_argument);
    }

    char* postfix = NULL;
    state st = get_postfix(argv[1], &postfix);
    if(st != well)
    {
        print_state(st);
        return 0;
    }
    //printf("%s\n", postfix);

    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if(tree == NULL)
    {
        free(postfix);
        print_state(memory_problem);
        return 0;
    }
    tree->root = NULL;
    st = build_tree(postfix, tree);
    if(st != well)
    {
        free(postfix);
        free(tree);
        print_state(st);
        return 0;
    }
    //print_tree(tree->root, 0);

    char* unique_symbols = NULL;
    int count_uniq = count_unique_symbols(postfix, &unique_symbols);
    if(count_uniq < 1 )
    {
        free(postfix);
        free_tree(tree->root);
        free(tree);
        print_state(invalid_symbol);
        return 0;
    }
    //printf("%s\n", unique_symbols);

    st = print_table(tree->root, count_uniq, unique_symbols);
    if(st != well)
    {
        free(postfix);
        free_tree(tree->root);
        free(tree);
        free(unique_symbols);
        print_state(st);
        return 0;
    }

    print_state(well);
    free(unique_symbols);
    free_tree(tree->root);
    free(tree);
    free(postfix);

}

