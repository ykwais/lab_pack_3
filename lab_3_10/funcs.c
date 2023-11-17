#include "helper.h"

input_status check_input(int argc, char** argv)
{
    if(argc != 3){
        return ic_wrong_count_args;
    }
    if(!strcmp(argv[1], argv[2])){

        return ic_same_file;
    }

    FILE* input_file = NULL;
    if(!(input_file = fopen(argv[1], "r"))) {
        return ic_problem_open_file;
    }
    int q = fgetc(input_file);
    if(q == EOF){
        fclose(input_file);
        return ic_empty_file;
    }else{
        fseek(input_file, 0, SEEK_SET);
        fclose(input_file);
    }

    return ic_well;
}


void print_tree(node* root, FILE* output_file, int depth) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        fprintf(output_file, "  ");
    }

    fprintf(output_file, "%s\n", root->arg);

    if (root->kid != NULL) {
        print_tree(root->kid, output_file, depth + 1);
    }

    if (root->bro != NULL) {
        print_tree(root->bro, output_file, depth);
    }

}

void free_tree(node* root)
{
    if(root == NULL)
    {
        return;
    }
    free( root->arg);

    if (root->kid != NULL) {
        free_tree(root->kid);
    }

    if (root->bro != NULL) {
        free_tree(root->bro);
    }

    free(root);

}

node* create_node(char* str){
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
        return NULL;
    }

    new_node->arg = str;
    new_node->bro = NULL;
    new_node->kid=NULL;

    return new_node;
}


node* get_root(const char* string, state* stat)
{
    int stack_size = 2;

    int top = 0;
    int summ = 0;
    node** stack = (node**)malloc(sizeof(node*)*stack_size);
    if(stack == NULL)
    {
        *stat = mem_problem;
        return NULL;
    }

    int calcul = 0;

    while(string[calcul] != '\0')
    {

        char c = string[calcul];
        if(calcul == 0 && c == '('){
            calcul++;
            continue;
        }
        if(c == '(')
        {
            int counter = 0;
            calcul++;
            char* str = get_lexemma(string, &counter, &calcul);
            if(str == NULL){
                *stat = mem_problem;
                for(int i = 0; i < summ; ++i){
                    free(stack[i]);
                }
                free(stack);
                return NULL;
            }

            node* new_node = create_node(str);
            if(new_node == NULL){
                *stat = mem_problem;
                for(int i = 0; i < summ; ++i){
                    free(stack[i]);
                }
                free(stack);
                return NULL;
            }
            calcul += counter;

            top++;
            if(top >= summ){
                summ = top;
            }
            if(top >= stack_size-1){
                stack_size *= 2;
                node** tmp = (node**)realloc(stack, sizeof(node*)*stack_size);
                if(tmp == NULL){
                    *stat = mem_problem;
                    for(int i = 0; i < summ; ++i){
                        free(stack[i]);
                    }
                    free(stack);
                    return NULL;
                }else{
                    stack = tmp;
                }
            }
            stack[top] = new_node;
        }
        else if(c == ',')
        {
            int counter = 0;
            calcul++;
            char* str = get_lexemma(string, &counter, &calcul);
            if(str == NULL){
                *stat = mem_problem;
                for(int i = 0; i < summ; ++i){
                    free(stack[i]);
                }
                free(stack);
                return NULL;
            }
            node* new_node = create_node(str);
            if(new_node == NULL){
                *stat = mem_problem;
                for(int i = 0; i < summ; ++i){
                    free(stack[i]);
                }
                free(stack);
                return NULL;
            }
            calcul += counter;

            node* tmp = stack[top];
            if(tmp->bro != NULL){
                node* temp = tmp;
                while(tmp->bro != NULL){
                    tmp = tmp->bro;
                }
                tmp->bro = new_node;
                stack[top] = temp;
            }
            else{
                tmp -> bro = new_node;
                stack[top] = tmp;
            }
        }
        else if(c == ')')
        {
            if(string[calcul+1] == '\0' && string[0] == '('){
                calcul++;
                continue;
            }
            node* tmp_1 = stack[top];
            top--;
            node* tmp_2 = stack[top];


            node* tmp_3 = tmp_2;
            while(tmp_2->bro != NULL){
                tmp_2 = tmp_2->bro;
            }
            tmp_2->kid = tmp_1;
            stack[top] = tmp_3;

            calcul++;

        }
        else
        {
            int counter = 0;
            char* str = get_lexemma(string, &counter, &calcul);
            if(str == NULL){
                *stat = mem_problem;
                for(int i = 0; i < summ; ++i){
                    free(stack[i]);
                }
                free(stack);
                return NULL;
            }
            node* new_node = create_node(str);
            if(new_node == NULL){
                *stat = mem_problem;
                for(int i = 0; i < summ; ++i){
                    free(stack[i]);
                }
                free(stack);
                return NULL;
            }
            calcul += counter;

            if(top >= stack_size-1){
                stack_size *= 2;
                node** tmp = (node**) realloc(stack, sizeof(node*)*stack_size);
                if(tmp == NULL){
                    *stat = mem_problem;
                    for(int i = 0; i < summ; ++i){
                        free(stack[i]);
                    }
                    free(stack);
                    return NULL;
                }else{
                    stack = tmp;
                }
            }
            stack[top] = new_node;

        }

    }
    node* root = stack[0];
    *stat = st_well;
    return root;

}

char* get_lexemma(const char* string, int* counter, const int* calcul)
{
    *counter = 0;
    int size = 2;
    char* strok = (char*)malloc(sizeof(char)*size);
    if(strok == NULL)
    {
        return NULL;
    }

    while(string[(*calcul)+(*counter)] != '\0' && string[(*calcul)+(*counter)] != '(' && string[(*calcul)+(*counter)] != ',' && string[(*calcul)+(*counter)] != ')')
    {
        if((*counter) >= size-1)
        {
            size *= 2;
            char* tmp = (char*)realloc(strok, size);
            if(tmp == NULL){

                free(strok);
                return NULL;
            }
            else{
                strok = tmp;
            }
        }
        strok[(*counter)] = string[(*calcul)+(*counter)];
        (*counter)++;
    }
    strok[(*counter)] = '\0';
    return strok;
}
