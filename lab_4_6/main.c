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

int get_value_from_tree(Node* root, char* unique_symbols, int count, int* val)
{
    if(root == NULL)
    {
        return -1;
    }

    int left_val = get_value_from_tree(root->left, unique_symbols, count, val);
    int right_val = get_value_from_tree(root->right, unique_symbols, count, val);

    if(isalpha(root->symbol))
    {
        for(int i = 0; i < count; ++i)
        {
            if(unique_symbols[i] == root->symbol)
            {
                return val[i];
            }
        }
    }

    if (root->symbol == '0' || root->symbol == '1')
    {
        return root->symbol - '0';
    }
    if(root->symbol == '&')
    {
        return left_val & right_val;
    }
    if(root->symbol == '|')
    {
        return left_val | right_val;
    }
    if(root->symbol == '=')
    {
        return (int)(left_val == right_val);
    }
    if(root->symbol == '~')
    {
        return !left_val;
    }
    if(root->symbol == '+')
    {
        return ~(~left_val | right_val);
    }
    if(root->symbol == '-')
    {
        return left_val <= right_val;
    }
    if(root->symbol == '<')
    {
        return (int)(left_val != right_val);
    }
    if(root->symbol == '!')
    {
        return !(left_val & right_val);
    }
    if(root->symbol == '?')
    {
        return !(left_val | right_val);
    }

    return -1;

}

state print_table(Node* root, int count, char* unique_symbols)
{

    char* new_filename = generate_filename();
    if(new_filename == NULL)
    {
        return memory_problem;
    }

    FILE* file = fopen(new_filename, "w");
    if(file == NULL)
    {
        free(new_filename);
        return memory_problem;
    }

    int amount_combo = 1 << count;
    int arr_var[amount_combo];

    for(int i = 0; i < amount_combo; ++i)
    {
        fprintf(file, "------------------\n");
        for(int j = 0; j < count; ++j)
        {
            int bit = (i >> j) & 1;
            fprintf(file, " %d |", bit);
            arr_var[j] = bit;
        }

        int final = get_value_from_tree(root, unique_symbols, count, arr_var);
        if(final == -1)
        {
            return invalid_symbol;
        }
        fprintf(file, " %d\n", final);
    }
    fprintf(file, "------------------\n");
    fclose(file);
    free(new_filename);

    return well;

}

bool not_used_symbol(const char* unique_symbols, char symbol, int count)
{
    for(int i = 0; i < count; i++)
    {
        if(unique_symbols[i] == symbol)
        {
            return false;
        }
    }
    return true;
}

int count_unique_symbols(char* postfix, char** unique_symbols)
{

    int count = 0;
    int buf = 2;
    *unique_symbols = (char*)malloc(sizeof(char)*(buf + 1));
    if(*unique_symbols == NULL)
    {
        return -1;
    }
    char* ptr = postfix;
    while(*ptr != '\0')
    {
        if(isalpha(*ptr) && not_used_symbol(*unique_symbols, *ptr, count))
        {
            if(count >= buf - 2)
            {
                buf *= 2;
                char* tmp = (char*)realloc(*unique_symbols, sizeof(char)*(buf + 1));
                if(tmp == NULL)
                {
                    free(*unique_symbols);
                    return -1;
                }
                *unique_symbols = tmp;
            }
            (*unique_symbols)[count++] = *ptr;
        }
        ptr++;
    }
    (*unique_symbols)[count] = '\0';
    return count;
}

void print_tree(Node* root, int level)
{
    if (root == NULL) {
        return;
    }

    print_tree(root->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%c\n", root->symbol);
    print_tree(root->left, level + 1);
}

bool is_operator(char symbol)
{
    return (symbol == '&' || symbol == '|' || symbol == '?' || symbol == '<' || symbol == '~' || symbol == '+' || symbol == '-' || symbol == '=' || symbol == '!');
}

Node* create_node_tree(char symbol)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL)
    {
        return NULL;
    }
    node->symbol = symbol;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree_stack(Tree_stack* stack)
{
    while(stack->top != NULL)
    {
        pop_tree_stack(stack);
    }
    free(stack);
}

void free_tree_stack_full(Tree_stack* stack)
{
    while(stack->top != NULL)
    {
        pop_tree_stack_full(stack);
    }
    free(stack);
}

void free_tree(Node* root)
{
    if(root == NULL)
    {
        return;
    }
    if(root->left != NULL) {
        free_tree(root->left);
    }
    if(root->right != NULL) {
        free_tree(root->right);
    }
    free(root);
}

state build_tree(char* postfix, Tree* tree)
{
    Tree_stack* stack = (Tree_stack*)malloc(sizeof(Tree_stack));
    if(stack == NULL)
    {
        return memory_problem;
    }
    stack->top = NULL;
    stack->bottom = NULL;

    state st;
    char* ptr = postfix;
    while(*ptr != '\0')
    {
        if(isalpha(*ptr) || *ptr == '1' || *ptr == '0')
        {
            Node* node = create_node_tree(*ptr);
            if(node == NULL)
            {
                free_tree_stack_full(stack);
                return memory_problem;
            }
            st = push_tree_stack(stack, node);
            if(st != well)
            {
                free(node);
                free_tree_stack_full(stack);
                return st;
            }
        }
        else if(is_operator(*ptr))
        {
            if(*ptr == '~')
            {
                Node* node = create_node_tree(*ptr);
                if(node == NULL)
                {
                    free_tree_stack_full(stack);
                    return memory_problem;
                }
                if(stack->top == NULL)
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return wrong_operation;
                }
                Node* left = stack->top->node;
                st = pop_tree_stack(stack);
                if(st != well)
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
                node->left = left;
                st = push_tree_stack(stack, node);
                if(st != well)
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
            }
            else
            {
                Node* node = create_node_tree(*ptr);
                if(node == NULL)
                {
                    free_tree_stack_full(stack);
                    return memory_problem;
                }
                if(stack->top == NULL )
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return wrong_operation;
                }
                Node* right = stack->top->node;
                st = pop_tree_stack(stack);
                if(st != well)
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
                if(stack->top == NULL )
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return wrong_operation;
                }
                Node* left = stack->top->node;
                st = pop_tree_stack(stack);
                if(st != well)
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
                node->left = left;
                node->right = right;
                st = push_tree_stack(stack, node);
                if(st != well)
                {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }

            }
        }
        else
        {
            free_tree_stack_full(stack);
            return invalid_symbol;
        }
        ptr++;
    }

    if(stack->top != stack->bottom)
    {
        free_tree_stack_full(stack);
        return wrong_operation;
    }
    tree->root = stack->top->node;


    free_tree_stack(stack);

    return well;

}



state push_tree_stack(Tree_stack* stack, Node* node)
{
    if(stack->bottom == NULL)
    {
        stack->bottom = (Node_stack *)malloc(sizeof(Node_stack));
        if(stack->bottom == NULL)
        {
            return memory_problem;
        }
        stack->bottom->node = node;
        stack->bottom->next = NULL;
        stack->top = stack->bottom;
        return well;
    }

    Node_stack* node_stack = (Node_stack*)malloc(sizeof(Node_stack));
    if(node_stack == NULL)
    {
        return memory_problem;
    }
    node_stack->node = node;
    node_stack->next = NULL;
    stack->top->next = node_stack;
    stack->top = node_stack;
    return well;
}

state pop_tree_stack_full(Tree_stack* stack)
{
    if (stack->bottom == NULL) {
        return invalid_argument;
    }
    if (stack->bottom->next == NULL) {
        free_tree(stack->bottom->node);
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return well;
    }
    Node_stack *tmp = stack->bottom;
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    stack->top = tmp;
    free_tree(tmp->next->node);
    free(tmp->next);
    tmp->next = NULL;
    return well;
}

state pop_tree_stack(Tree_stack* stack)
{
    if (stack->bottom == NULL) {
        return invalid_argument;
    }
    if (stack->bottom->next == NULL) {
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return well;
    }
    Node_stack *tmp = stack->bottom;
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    stack->top = tmp;
    free(tmp->next);
    tmp->next = NULL;
    return well;
}


char* generate_filename()
{
    srand(time(NULL));

    int len = MIN_LEN + rand() % (MAX_LEN - MIN_LEN + 1);

    char *file = (char *)malloc((len + 5) * sizeof(char));
    if(file == NULL)
    {
        return NULL;
    }

    char symbols[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int count_symbols = sizeof(symbols) - 1;

    for (int i = 0; i <= len; ++i)
    {
        int index = rand() % count_symbols;
        file[i] = symbols[index];
    }
    file[len] = '\0';
    strcat(file, ".txt");

    return file;
}