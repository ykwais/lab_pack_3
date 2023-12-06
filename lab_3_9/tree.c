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
//    int len = (int)strlen(str);
//    new_node->word = (char*) malloc(sizeof(char)*(len + 1));
//    if(new_node->word == NULL)
//    {
//        free(new_node);
//        return NULL;
//    }
//    strcpy(new_node->word, str);
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
    int comp = strcmp(root->word, new_node->word);
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
    int comp = strcmp(root->word, str);
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
    printf("%s<%d>\n", node->word, node->counter);
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