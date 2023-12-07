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
        if(new_node == NULL)
        {
            free(one_word);
            fclose(file);
            return meme_problem;
        }
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

        if(c == '\n' || c == '\r')//это для представления полноценного слова без \n внутри него
        {
            c = fgetc(file);
            continue;
        }

        if(c == '|'){
            free(result);
            *stat = rl_mem_problem;
            return NULL;
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

char* read_line(state* stat, FILE* file)
{

    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL){
        *stat = meme_problem;
        return NULL;
    }
    int ch;
    while((ch = fgetc(file)) != EOF && ch != '\n'){
        size++;
        if(size >= buf)
        {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL)
            {
                free(result);
                *stat = meme_problem;
                return NULL;
            }else{
                result = tmp;
            }
        }
        result[size-1] = (char)ch;
    }
    result[size] = '\0';
    if(strcmp(result, "\0") == 0 || strcmp(result, "\r") == 0 || strcmp(result, "\n") == 0){
        *stat = empty_str;
        free(result);
        return NULL;
    }
    *stat = well;
    return result;
}

void clear_input_buffer(FILE* file) {
    int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF);
}

state interactive(Tree** tree, int* amount_saved, int* buffer, char*** array_saved_files)
{
    char* menu[] = {
            "==============================================",
            "1 - amount of your word in file",
            "2 - first n most amount word",
            "3 - the longest word",
            "4 - the shortest word",
            "5 - depth of tree",
            "6 - print tree",
            "7 - save tree in file",
            "8 - load tree from file",
            "0 - exit"
    };

    while(1)
    {
        for(int i = 0; i < 10; ++i)
        {
            printf("%s\n", menu[i]);
        }


        char choice;
        fflush(stdin);
        if(scanf("%c", &choice) == EOF){
            break;
        }
        int c;
        if ((c = getchar()) != '\n' && c != EOF) {
            clear_input_buffer(stdin);
            printf("you've inputted wrong action, rerun\n");
            continue;
        }

        if(choice == '0'){
            printf("bye\n");
            break;
        }


        if(choice == '1')
        {
            printf("input the word you wanna find:\n");
            state stat = well;
            fflush(stdin);
            char *strok = read_line(&stat, stdin);
            if (stat == meme_problem) {
                return meme_problem;
            }
            if (stat == empty_str) {
                free(strok);
                printf("you've entered the empty string!\n");
                continue;
            }

            Node* node = find_node((*tree)->root, strok);
            if(node)
            {
                printf("the word: %s occurred <%d> times in input file\n", strok, node->counter);
            }
            else{
                printf("there is no such word in file\n");
            }
            free(strok);
            continue;
        }
        else if(choice == '2')
        {
            printf("input the n:\n");
            char num[10];
            fflush(stdin);
            scanf("%9s", num);
            int ch;
            if ((ch = getchar()) != '\n' && ch != EOF) {
                clear_input_buffer(stdin);
                printf("you've entered too large integer\n");
                continue;
            }
            char* endptr;
            int n = strtol(num, &endptr, 10);
            if (endptr == num || *endptr != '\0' || n - 2 > INT_MAX) {
                printf("not valid number\n");
                continue;
            }
            if(n < 1)
            {
                printf("the n cannot be a negative number or zero!\n");
                continue;
            }


            int buf = 1;
            arr_nodes array;
            array.nodes = (Node**)malloc(sizeof(Node*)*buf);
            if(array.nodes == NULL)
            {
                return meme_problem;
            }
            array.buf = 1;
            array.size = 0;
            state st = well;
            get_array((*tree)->root, &array, &st);
            if(st == meme_problem)
            {
                free(array.nodes);
                return meme_problem;
            }
            if(n > array.size)
            {
                printf("you have specified more elements than there are in the tree!\n");
                free(array.nodes);
                continue;
            }

            qsort(array.nodes, array.size, sizeof(Node*), comp_occur);

            printf("the n most amount words:\n");
            for(int i = 0; i < n; ++i)
            {
                printf("%s\n", array.nodes[i]->word);
            }

            free(array.nodes);



        }
        else if(choice == '3')
        {
            printf("max node\n");
            print_node(get_max((*tree)->root));
        }
        else if(choice == '4')
        {
            printf("min node\n");
            print_node(get_min((*tree)->root));
        }
        else if(choice == '5'){
            printf("depth of tree: %d\n", depth_tree((*tree)->root));
        }
        else if(choice == '6'){
            print_tree((*tree)->root, 1);
        }
        else if(choice == '7'){
            printf("Enter the name of file:\n");
            state stat = well;
            fflush(stdin);
            char *strok = read_line(&stat, stdin);
            if (stat == meme_problem) {
                return meme_problem;
            }
            if (stat == empty_str) {
                free(strok);
                printf("you've entered the empty string!\n");
                continue;
            }
            FILE* file = fopen(strok, "w");
            if(file == NULL){
                printf("can't open file: %s. Enter the other one\n", strok);
                free(strok);
                continue;
            }
            if(*tree == NULL || (*tree)->root == NULL){
                printf("tree is empty\n");
                fclose(file);
                free(strok);
                continue;
            }
            save_tree((*tree), file);
            *amount_saved += 1;
            if( *amount_saved >= *buffer){
                *buffer *= 2;
                char** tmp = (char**) realloc(*array_saved_files, sizeof(Node*)*(*buffer));
                if(tmp == NULL){
                    fclose(file);
                    free(strok);
                    return meme_problem;//подумать об очистке памяти массива
                }
                else{
                    *array_saved_files = tmp;
                }
            }
            (*array_saved_files)[*amount_saved - 1] = strok;
            fclose(file);
            printf("tree was successfully saved\n");
            continue;
        }
        else if(choice == '8'){
            if(*amount_saved == 0){
                printf("there is no saved files\n");
                continue;
            }
            printf("Enter the name of file:\n");
            state stat = well;
            fflush(stdin);
            char *strok = read_line(&stat, stdin);
            if (stat == meme_problem) {
                return meme_problem;
            }
            if (stat == empty_str) {
                free(strok);
                printf("you've entered the empty string!\n");
                continue;
            }
            int i;
            bool exist = false;
            for(i = 0; i < *amount_saved; ++i){
                if(strcmp(strok, (*array_saved_files)[i]) == 0){
                    delete_tree((*tree)->root);
                    free(*tree);
                    *tree = init_tree();
                    stat = build_tree_with_file(*tree, strok, "|");
                    if(stat == meme_problem){
                        return meme_problem;
                    }

                    printf("tree was successfully loaded\n");
                    exist = true;
                    break;
                }

            }
            if(!exist){

                printf("there is no such file\n");

            }
            free(strok);
            continue;
        }
        else{
            printf("you've inputted wrong action, rerun\n");
            continue;
        }
    }
    return well;
}

void insert_arr(arr_nodes* arr, Node* curr, state* st)
{
    arr->size++;
    if(arr->size >= arr->buf)
    {
        arr->buf *= 2;
        Node** tmp = (Node**) realloc(arr->nodes, sizeof(Node*)*arr->buf);
        if(tmp == NULL)
        {
            *st = meme_problem;

            return;
        }
        else{
            arr->nodes = tmp;
        }
    }
    arr->nodes[arr->size - 1] = curr;
    *st = well;
}

void get_array(Node* root, arr_nodes* array, state* st)
{
    if(*st == meme_problem)
    {
        return;
    }

    insert_arr(array, root, st);

    if(root->left != NULL)
    {
        get_array(root->left, array, st);
    }
    if(root->right != NULL)
    {
        get_array(root->right, array,st);
    }
}

int comp_occur(const void* a, const void* b)
{
    Node* first = *(Node**)a;
    Node* second = *(Node**)b;

    return second->counter - first->counter;
}

Node* get_max(Node* root)
{
    if(root->right == NULL)
    {
        return root;
    }
    return get_max(root->right);
}

Node* get_min(Node* root)
{
    if(root->left == NULL)
    {
        return root;
    }
    return get_min(root->left);
}
