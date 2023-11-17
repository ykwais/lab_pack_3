#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//TODO reallod for stack

typedef struct node{
    char* arg;
    struct node* kid;
    struct node* bro;
}node;

typedef enum input_code{
    ic_well,
    ic_problem_open_file,
    ic_empty_file,
    ic_wrong_count_args,
    ic_same_file
}input_status;

input_status check_input(int argc, char** argv);
node* get_root(const char* string);
//node* create_node(node* parent, char* str);
//void add_child(node* parent, node* kid);
void print_tree(node* root, FILE* outputFile, int depth);
char* get_lexemma(const char* string, int* counter, const int* calcul);



int main(int argc, char** argv) {
    switch (check_input(argc, argv)) {

        case ic_same_file:
            printf("You've inputted the same file!\n");
            return 0;
        case ic_wrong_count_args:
            printf("wrong number of arguments!\n");
            return 0;
        case ic_empty_file:
            printf("You've entered the empty file!\n");
            return 0;
        case ic_problem_open_file:
            printf("problem with opening the file: %s\n", argv[1]);
            return 0;
        case ic_well:
            break;
    }


    FILE* in_file = NULL;
    FILE* out_file = NULL;

    if(!(in_file = fopen(argv[1], "r")))
    {
        printf("problem with opening the file: %s\n", argv[1]);
        return 0;
    }

    if(!(out_file = fopen(argv[2], "w")))
    {
        printf("problem with opening the file: %s\n", argv[2]);
        fclose(out_file);
        return 0;
    }

    int counter = 0;
    int size = 2;
    int c = 0;

    while((c = fgetc(in_file) )!= EOF)
    {
        char* strok = (char*)malloc(sizeof(char)*size);
        if(strok == NULL)
        {
            printf("problem with memory allocation!\n");
            fclose(in_file);
            fclose(out_file);
            return 0;
        }
        counter++;
        strok[counter-1] = (char)c;
        while((c = fgetc(in_file)) != '\n' && c != EOF)
        {
            counter++;
            char* tmp = NULL;
            if(counter >= size)
            {
                size *= 2;
                tmp = (char*)realloc(strok, sizeof(char)*size);
                if(tmp == NULL){
                    printf("problem with memory allocation!\n");
                    free(strok);
                    fclose(in_file);
                    fclose(out_file);
                    return 0;
                }
                else{
                    strok = tmp;
                }
            }
            strok[counter - 1] = (char)c;
        }
        strok[counter] = '\0';
        printf("%s\n", strok);
        node* root = get_root(strok);
        print_tree(root, out_file, 0);


        counter = 0;
        size = 2;
        free(strok);
    }

    fclose(in_file);
    fclose(out_file);
}


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

node* create_node(char* str){
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
        printf("meeeeeeem alloc prblem!\n");
        return NULL;
    }

    new_node->arg = str;
    new_node->bro = NULL;
    new_node->kid=NULL;
    return new_node;
}

//void add_child(node* parent, node* kid)
//{
//    if(parent->children == NULL)
//    {
//        parent->children = kid;
//        kid->parent = parent;
//    }
//    else{
//        node* brother = parent->children;
//        while(brother->bro != NULL){
//            brother = brother->bro;
//        }
//        brother->bro = kid;
//        kid->parent = parent;
//    }
//}

node* get_root(const char* string)
{
    int stack_size = 50;
    int top = 0;
    node** stack = (node**)malloc(sizeof(node*)*stack_size);
    if(stack == NULL)
    {
        printf("mem problem stack\n");
        return NULL;
    }

    int calcul = 0;

    while(string[calcul] != '\0')
    {

        char c = string[calcul];
        if(c == '(')
        {
            int counter = 0;
            calcul++;
            char* str = get_lexemma(string, &counter, &calcul);
            node* new_node = create_node(str);
            calcul += counter;

            top++;
            if(top >= stack_size-1){
                stack_size *= 2;
                node** tmp = (node**)realloc(stack, stack_size);
                if(tmp == NULL){
                    printf("meme realloc problem\n");
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
            node* new_node = create_node(str);
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
            node* tmp_1 = stack[top];
            top--;
            node* tmp_2 = stack[top];
            if(tmp_2->kid != NULL){
                node* tmp_3 = tmp_2;
                while(tmp_2->kid != NULL){
                    tmp_2 = tmp_2->bro;
                }
                tmp_2->kid = tmp_1;
                stack[top] = tmp_3;
            }
            else{
                tmp_2->kid = tmp_1;
                stack[top] = tmp_2;
            }

            calcul++;

        }
        else
        {
            int counter = 0;
            char* str = get_lexemma(string, &counter, &calcul);
            node* new_node = create_node(str);
            calcul += counter;

            if(top >= stack_size-1){
                stack_size *= 2;
                node** tmp = (node**) realloc(stack, stack_size);
                if(tmp == NULL){
                    printf("meme realloc problem\n");
                    return NULL;
                }else{
                    stack = tmp;
                }
            }
            stack[top] = new_node;


        }

    }
    node* root = stack[0];
    return root;


}

char* get_lexemma(const char* string, int* counter, const int* calcul)
{
    *counter = 0;
    int size = 2;
    char* strok = (char*)malloc(sizeof(char)*size);
    if(strok == NULL)
    {
        printf("malloc strok problem!\n");
        return NULL;
    }


    while(string[(*calcul)+(*counter)] != '\0' && string[(*calcul)+(*counter)] != '(' && string[(*calcul)+(*counter)] != ',' && string[(*calcul)+(*counter)] != ')')
    {
        if((*counter) >= size-1)
        {
            size *= 2;
            char* tmp = (char*)realloc(strok, size);
            if(tmp == NULL){
                //mem trouble
                printf("meme trouble\n");
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


void print_tree(node* root, FILE* outputFile, int depth) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        fprintf(outputFile, "  ");
    }

    fprintf(outputFile, "%s\n", root->arg);

    node* child = root->kid;
    while (child != NULL) {
        print_tree(child, outputFile, depth + 1);
        child = child->bro;
    }
//    fprintf(outputFile,"\n");


}
