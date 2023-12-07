#include "helper.h"

Tree* init_tree()
{
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if(tree == NULL)
    {
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

Node* create_node(char* str)
{
    Node* new_node = (Node*) malloc(sizeof(Node));
    if(new_node == NULL)
    {
        return NULL;
    }
    new_node->word = str;
    new_node->counter = 1;
    new_node->right = NULL;
    new_node->left = NULL;
    return new_node;
}

void delete_node(Node* current_node)
{
    if(current_node == NULL)
    {
        return;
    }
    free(current_node->word);
    free(current_node);
}

void delete_tree(Node* root)
{
    if(root != NULL)
    {
        delete_tree(root->left);
        delete_tree(root->right);
        delete_node(root);
    }
}

Node* add_node(Node* root, Node* new_node, new_old* st)
{
    if(root == NULL)
    {
        return new_node;
    }

    int comp = comp_word(root->word, new_node->word);
    if(comp == 0)
    {
        root->counter++;
        delete_node(new_node);
        *st = old;
    }
    else if(comp > 0)
    {
        root->left = add_node(root->left, new_node, st);
    }
    else
    {
        root->right = add_node(root->right, new_node, st);
    }
    return root;
}

Node* find_node(Node* root, const char* str)
{
    if(root == NULL)
    {
        return NULL;
    }
    //int comp = strcmp(root->word, str);
    int comp = comp_word(root->word, str);
    if(comp == 0){
        return root;
    }
    else if(comp > 0)
    {
        return find_node(root->left, str);
    }
    else
    {
        return find_node(root->right, str);
    }

}

void print_node(Node* node)
{
    if(node != NULL){
        printf("%s<%d>\n", node->word, node->counter);
    }else{
        printf("NULL\n");
    }

}

void print_tree(Node* root, int depth)
{
    if(root != NULL)
    {
        print_tree(root->right, depth + 1);
        for(int i = 0; i < depth; ++i)
        {
            printf("\t");
        }
        print_node(root);
        print_tree(root->left, depth + 1);
    }
}

int comp_word(const char* a, const char* b)
{

    if(strlen(a) == strlen(b))
    {return strcmp(a, b); }

    return (int)strlen(a) - (int)strlen(b);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int depth_tree(Node* root)
{
    if(root == NULL)
    {
        return 0;
    }
    return 1 + max(depth_tree(root->left), depth_tree(root->right));
}

void save_tree_node(Node* root, FILE* file)
{
    if(root == NULL || file == NULL)
    {
        return;
    }
    for(int i = 0; i < root->counter; ++i){
        fprintf(file, "%s|", root->word);
    }

    save_tree_node(root->left, file);
    save_tree_node(root->right, file);
}

void save_tree(Tree* tree, FILE* file)
{
    if(tree == NULL || file == NULL)
    {
        return;
    }
    save_tree_node(tree->root, file);
}