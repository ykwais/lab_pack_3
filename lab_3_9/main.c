
#include "helper.h"



void print_status(state st)
{
    switch(st)
    {
        case well:
            break;
        case meme_problem:
            printf("problem with memory allocation or you've inputted character '|' in input file!\n");
            break;
        case empty_file:
            printf("you've inputted empty file!\n");
            break;

        case problem_with_file_open:
            printf("problem with file open!\n");
            break;

        default:
            printf("Unknown status!\n");
            break;
    }
}



int main(int argc, char** argv) {
    if(argc < 3){
        printf("too little arguments!\n");
        return 0;
    }
    int buf = 1;
    int size = 0;
    char* separators = (char*)malloc(sizeof(char)*(buf+1));

    if(separators == NULL)
    {
        print_status(meme_problem);
        return 0;
    }

    for(int i = 2; i < argc; ++i)
    {
        if((strlen(argv[i]) > 1 && ( strcmp(argv[i], "\\n") != 0 && strcmp(argv[i], "\\t") != 0 && strcmp(argv[i], "\\r") != 0)) || argv[i][0] == '|'){
            printf("wrong separator!\n");// | зарезервирован для восстановления дерева из файла
            free(separators);
            return 0;
        }
        size++;
        if(size >= buf)
        {
            buf *= 2;
            char* tmp = (char*) realloc(separators, sizeof(char)*(buf+1));
            if(tmp == NULL)
            {
                free(separators);
                print_status(meme_problem);
                return 0;
            }
            else
            {
                separators = tmp;
            }
        }
        if(argv[i][0] == '\\')
        {
            if(argv[i][1] == 'n')
                separators[size-1] = '\n';
            else if(argv[i][1] == 't')
                separators[size-1] = '\t';
            else if(argv[i][1] == 'r')
                separators[size - 1] = '\r';
        }else {
            separators[size - 1] = argv[i][0];
        }
    }
    separators[size] = '\0';


    Tree* tree = init_tree();
    if(tree == NULL)
    {
        free(separators);
        print_status(meme_problem);
        return 0;
    }


    state stat = build_tree_with_file(tree, argv[1], separators);
    if(stat != well)
    {
        print_status(stat);
        free(separators);
        delete_tree(tree->root);
        free(tree);
        return 0;
    }

    int amount_saved = 0;
    int buffer = 1;
    char** array_saved_files = (char**)malloc(sizeof(char*)*(buffer));
    if(array_saved_files == NULL)
    {
        free(separators);
        delete_tree(tree->root);
        free(tree);
        print_status(meme_problem);
        return 0;
    }
    //print_tree(tree->root, 1);
    stat = interactive(&tree, &amount_saved, &buffer ,&array_saved_files);
    if(stat != well)
    {
        print_status(stat);
        free(separators);
        delete_tree(tree->root);
        free(tree);
        free(array_saved_files);
        return 0;
    }

    free(separators);
    delete_tree(tree->root);
    free(tree);
    for(int i = 0; i < amount_saved; ++i)
    {
        free(array_saved_files[i]);
    }
    free(array_saved_files);
}

