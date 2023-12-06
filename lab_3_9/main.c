
#include "helper.h"





void print_status(state st)
{
    switch(st)
    {
        case well:
            break;
        case meme_problem:
            printf("problem with memory allocation!\n");
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
        char* arg = argv[i];
        if((strlen(argv[i]) > 1 && (argv[i][0] != '\\') && (argv[i][1] != 'n' || argv[i][1] != 'r'|| argv[i][1] != 't')) || strlen(argv[i]) > 2){
            printf("wrong separator! check your data\n");
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
        print_status(meme_problem);
        return 0;
    }


    build_tree_with_file(tree, argv[1], separators);
    print_tree(tree->root, 1);
    free(separators);
    delete_tree(tree->root);
    free(tree);
}



//state build_tree_with_file(Tree* tree, const char* filename, const char* separators)
//{
//    FILE* file = fopen(filename, "r");
//    if(file == NULL)
//    {
//        return problem_with_file_open;
//    }
//    int q = fgetc(file);
//    if (q == EOF) {
//        fclose(file);
//        return empty_file;
//    } else {
//        fseek(file, 0, SEEK_SET);
//    }
//
//    rl_status stat = rl_well;
//    char* one_word = NULL;
//    while( (one_word = get_word(file, separators, &stat)) != NULL && stat != rl_eof && stat != rl_mem_problem)
//    {
//        Node* new_node = create_node(one_word);
//        new_old no;
//        tree->root = add_node(tree->root, new_node, &no);
//
//    }
//
//    if(stat == rl_mem_problem)
//    {
//        fclose(file);
//        return meme_problem;
//    }
//    fclose(file);
//    return well;
//}
//
//
//char* get_word(FILE* file, const char* separators, rl_status* stat)
//{
//    int c;
//    int buf = 1;
//    char* result = (char*)malloc(sizeof(char)*(buf + 1));
//    if(result == NULL)
//    {
//        *stat = rl_mem_problem;
//        return NULL;
//    }
//    int size = 0;
//    do
//    {
//        c = fgetc(file);
//    } while (c != EOF && strchr(separators, c));
//
//    while(c != EOF && strchr(separators, c) == NULL)
//    {
//        if(c == '\n')
//        {
//            c = fgetc(file);
//            continue;
//        }
//        size++;
//        if(size >= buf){
//            buf *= 2;
//            char* tmp = (char*) realloc(result, sizeof(char)*(buf + 1));
//            if(tmp == NULL)
//            {
//                free(result);
//                *stat = rl_mem_problem;
//                return NULL;
//            }
//            else{
//                result = tmp;
//            }
//        }
//        result[size - 1] = (char)c;
//        c = fgetc(file);
//
//    }
//
//    if(size == 0 && c == EOF){
//        *stat = rl_eof;
//        free(result);
//        return NULL;
//    }
//
//    result[size] = '\0';
//    return result;
//
//}

//    char* one_string = NULL;
//    while((one_string = read_line(&stat, file)) != NULL && (stat != rl_eof && stat != rl_mem_problem ))
//    {
//        char* token = strtok(one_string, separators);
//        while(token != NULL){
//            //TODO создание нового узла через копию токена
//            int len = strlen(token);
//            char* word = (char*)malloc(sizeof(char)*(len + 1));
//            if(word == NULL)
//            {
//                fclose(file);
//                free(one_string);
//                return meme_problem;
//            }
//            strcpy(word, token);
//            Node* new_node = create_node(word);
//            new_old no;
//            tree->root = add_node(tree->root, new_node, &no);
//            token = strtok(NULL, separators);
//        }
//    }

//char* read_line(rl_status* stat, FILE* file)
//{
//
//    int size = 0;
//    int buf = 10;
//    char* result = (char*)malloc(sizeof(char)*(buf+1));
//    if(result == NULL){
//        *stat = rl_mem_problem;
//        return NULL;
//    }
//    int ch;
//    while((ch = fgetc(file)) != EOF && ch != '\n'){
//        size++;
//        if(size >= buf)
//        {
//            buf *= 2;
//            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
//            if(tmp == NULL)
//            {
//                free(result);
//                *stat = rl_mem_problem;
//                return NULL;
//            }else{
//                result = tmp;
//            }
//        }
//        result[size-1] = (char)ch;
//    }
//    if(size == 0 && ch == EOF){
//        *stat = rl_eof;
//        free(result);
//        return NULL;
//    }
//    result[size] = '\0';
//    if(strcmp(result, "\0") == 0 || strcmp(result, "\r") == 0){
//        *stat = rl_empty_str;
//        free(result);
//        return NULL;
//    }
//    *stat = rl_well;
//    return result;
//}