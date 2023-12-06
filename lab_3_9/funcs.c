#include "helper.h"

state build_tree_with_file(Tree* tree, const char* filename, const char* separators)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        return problem_with_file_open;
    }
    int q = fgetc(file);
    if (q == EOF) {
        fclose(file);
        return empty_file;
    } else {
        fseek(file, 0, SEEK_SET);
    }

    rl_status stat = rl_well;
    char* one_word = NULL;
    while( (one_word = get_word(file, separators, &stat)) != NULL && stat != rl_eof && stat != rl_mem_problem)
    {
        Node* new_node = create_node(one_word);
        new_old no;
        tree->root = add_node(tree->root, new_node, &no);

    }

    if(stat == rl_mem_problem)
    {
        fclose(file);
        return meme_problem;
    }
    fclose(file);
    return well;
}


char* get_word(FILE* file, const char* separators, rl_status* stat)
{
    int c;
    int buf = 1;
    char* result = (char*)malloc(sizeof(char)*(buf + 1));
    if(result == NULL)
    {
        *stat = rl_mem_problem;
        return NULL;
    }
    int size = 0;
    do
    {
        c = fgetc(file);
    } while (c != EOF && strchr(separators, c));

    while(c != EOF && strchr(separators, c) == NULL)
    {
        if(c == '\n')
        {
            c = fgetc(file);
            continue;
        }
        size++;
        if(size >= buf){
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf + 1));
            if(tmp == NULL)
            {
                free(result);
                *stat = rl_mem_problem;
                return NULL;
            }
            else{
                result = tmp;
            }
        }
        result[size - 1] = (char)c;
        c = fgetc(file);

    }

    if(size == 0 && c == EOF){
        *stat = rl_eof;
        free(result);
        return NULL;
    }

    result[size] = '\0';
    return result;

}