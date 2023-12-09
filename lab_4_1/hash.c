#include "helper.h"

state get_hash(char* key, HashTable* ht, unsigned long long* result)
{
    unsigned long long hash = 0;
    int base = 62;

    int len = (int)strlen(key);
    for(int i = len-1; i >= 0; i--)
    {
        int value;
        if(key[i] >= 'a' && key[i] <= 'z')
        {
            value = key[i] - 'a' + 10;
        }
        else if(key[i] >= 'A' && key[i] <= 'Z')
        {
            value = key[i] - 'A' + 36;
        }
        else if(key[i] >= '0' && key[i] <= '9')
        {
            value = key[i] - '0';
        }
        else{
            return invalid_data;
        }
        if((ULLONG_MAX - hash)/base < value){
            return invalid_data;
        }
        hash = hash * base + value;
    }

    *result = hash % ht->size;

    return well;
}



state insert_in_ht(HashTable* ht, char* key, char* value)
{
    int len1 = (int)strlen(value);
    int len2 = (int)strlen(key);
    if(len1 > 0 && value[len1-1] == '\n')
    {
        value[len1-1] = '\0';
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL)
    {
        return meme_problem;
    }

    new_node->key = (char*)malloc(sizeof(char)*(len2+1));
    if(new_node->key == NULL)
    {
        free(new_node);
        return meme_problem;
    }
    strcpy(new_node->key, key);
    new_node->value = (char*)malloc(sizeof(char)*(len1+1));
    if(new_node->value == NULL)
    {
        free(new_node->key);
        free(new_node);
        return meme_problem;
    }
    strcpy(new_node->value, value);

    unsigned long long int index;
    state st = get_hash(key, ht, &index);
    if(st != well){
        free(new_node);
        free(key);
        free(value);
        return st;
    }

    new_node->next = ht->table[index];
    ht->table[index] = new_node;

    return well;


}

HashTable* create_hashtable(unsigned int size)
{
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if(ht == NULL)
    {
        return NULL;
    }
    ht->size = size;
    ht->table = (Node**)calloc(size, sizeof(Node*));
    if(ht->table == NULL)
    {
        free(ht);
        return NULL;
    }
    return ht;
}

void remove_hashtable(HashTable* ht)
{
    if(ht != NULL)
    {
        for(int i = 0; i < ht->size; i++){
            Node* tmp = ht->table[i];
            while(tmp != NULL){
                Node* tmp_next = tmp->next;
                free(tmp->key);
                free(tmp->value);
                free(tmp);
                tmp = tmp_next;
            }
        }
        free(ht->table);
        free(ht);
    }
}





state resize_ht(HashTable** ht, unsigned int new_size) {
    HashTable *tmp = create_hashtable(new_size);
    if (tmp == NULL) {
        return meme_problem;
    }

    for(int i = 0; i < (*ht)->size; i++)
    {
        Node* tmp_node = (*ht)->table[i];
        while(tmp_node != NULL)
        {
            state st = insert_in_ht(tmp, tmp_node->key, tmp_node->value);
            if(st != well){
                remove_hashtable(tmp);
                return st;
            }
            tmp_node = tmp_node->next;
        }
    }
    remove_hashtable(*ht);
    *ht = tmp;
    return well;
}

state balance_insert(HashTable** ht, char* key, char* value)
{
    state st = insert_in_ht(*ht, key, value);
    if(st != well)
    {
        return st;
    }

    int min_len_list = INT_MAX;
    int max_len_list = 0;

    for(int i = 0; i < (*ht)->size; i++)
    {
        Node* tmp = (*ht)->table[i];
        int len_list = 0;
        while(tmp != NULL){
            len_list++;
            tmp = tmp->next;
        }
        if(len_list > max_len_list)
        {
            max_len_list = len_list;
        }
        if(len_list < min_len_list && len_list != 0)
        {
            min_len_list = len_list;
        }
    }

    if(max_len_list >= 2*min_len_list)
    {
        st = resize_ht(ht, 2*(*ht)->size);
        if(st != well){
            return st;
        }
    }
    return well;
}