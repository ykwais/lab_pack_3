#include "helper.h"


void print_status(state status)
{
    switch (status)
    {
        case well:
            printf("that's well\n");
            break;
        case probem_file_opening:
            printf("problem to open file\n");
            break;
        case file_wrote:
            printf("unable to write to the file\n");
            break;
        case file_read:
            printf("unable to read the file.\n");
            break;
        case same_file:
            printf("files are the same.\n");
            break;
        case mem_problem:
            printf("memory alloc problem\n");
            break;
        case invalid_comand:
            printf("invalid command.\n");
            break;
        case invalid_index:
            printf("invalid index.\n");
            break;
        case invalid_range:
            printf("invalid range.\n");
            break;
        case empty_array:
            printf( "the array is empty.\n");
            break;
        case not_existed_array:
            printf("the array does not exist.\n");
            break;
        case exist_array:
            printf("the array already exists.\n");
            break;
        case int_overflow:
            printf( "int overflow.\n");
            break;
        case invalid_format:
            printf("invalid format.\n");
            break;
        case invalid_count_argc:
            printf("invalid count argc.\n");
            break;
        case empty_file:
            printf("the file is empty.\n");
            break;
        default:
            printf("unknown status.\n");
            break;
    }
}


state free_array(Massive** array)
{
    if(*array == NULL)
    {
        return empty_array;
    }
    if((*array)->data != NULL)
    {
        free((*array)->data);
    }
    (*array)->data = NULL;
    (*array)->size = 0;
    (*array)->buf = 0;
    free(*array);
    *array = NULL;
    return well;
}

state create_new_array(Massive** arr, char name, int buf)
{
    if(buf <= 0)
    {
        return invalid_comand;
    }
    *arr = (Massive*)malloc(sizeof(Massive));
    if(*arr == NULL)
    {
        return mem_problem;
    }
    (*arr)->data = (int*)malloc(sizeof(int) * buf);
    if((*arr)->data == NULL)
    {
        free(*arr);
        *arr = NULL;
        return mem_problem;
    }
    (*arr)->size = 0;
    (*arr)->buf = buf;
    (*arr)->name = name;
    return well;
}

void free_all_arrays(Massive* array_storage[], int size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (array_storage[i] != NULL)
        {
            free_array(&array_storage[i]);
        }
    }

}

state get_arr_from_file(char* filename, Massive* arr)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return probem_file_opening;
    }

    char* line = NULL;
    size_t len = 0;

    while(getline(&line, &len, file) != -1)
    {
        char* token = strtok(line, " \n\t\r");
        while(token != NULL)
        {
            int value;

            state st = string_to_int(token, &value);
            if(st != well)
            {
                fclose(file);
                free(line);
                return st;
            }

            if(arr->size == arr->buf)
            {
                arr->buf *= 2;
                int* tmp = (int*)realloc(arr->data, sizeof(int) * arr->buf );
                if(tmp == NULL)
                {
                    fclose(file);
                    free(line);
                    return mem_problem;
                }
                arr->data = tmp;

            }
            arr->data[arr->size++] = value;
            token = strtok(NULL, " \n\t\r");

        }
    }
    fclose(file);
    free(line);
    if(arr->size == 0)
    {
        return empty_file;
    }
    return well;

}

state string_to_int(char* str, int* value)
{
    char* end;
    errno = 0;
    long int conv = strtol(str, &end, 10);

    if(end == str || (*end != '\r' && *end != '\n' && *end != '\0'))
    {
        return invalid_format;
    }
    if ((errno == ERANGE && (conv == LONG_MAX || conv == LONG_MIN)) || (errno != 0 && conv == 0)) {
        return int_overflow;
    }

    if(conv > INT_MAX || conv < INT_MIN)
    {
        return int_overflow;
    }

    *value = (int)conv;
    return well;
}

state save_to_file(char* filename, Massive* arr)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return probem_file_opening;
    }
    for(int i = 0; i < arr->size; i++)
    {
        fprintf(file, "%d ", arr->data[i]);

    }
    fclose(file);
    return well;
}


state fill_randomly(Massive* arr, int lb, int rb, int count)
{
    if(lb > rb)
    {
        return invalid_range;
    }
    if(count > arr->buf)
    {
        arr->buf = count;
        int* tmp = (int*)realloc(arr->data, sizeof(int) * arr->buf);
        if(tmp == NULL)
        {
            return mem_problem;
        }
        arr->data = tmp;
    }

    for(int i = 0; i < count; i++)
    {
        arr->data[i] = rand() % (rb - lb + 1) + lb;
    }
    arr->size = count;
    return well;
}

state concatination_arrays(Massive* arr_first, Massive* arr_second)
{
    int new_size = arr_first->size + arr_second->size;
    if(new_size > arr_first->buf)
    {
        int* tmp = (int*)realloc(arr_first->data, sizeof(int) * new_size);
        if(tmp == NULL)
        {
            return mem_problem;
        }
        arr_first->data = tmp;
    }
    for(int i = 0; i < arr_second->size; i++)
    {
        arr_first->data[arr_first->size + i] = arr_second->data[i];
    }
    arr_first->size = new_size;
    return well;
}

state remove_some_elem_from_array(Massive* arr, int start, int count)
{
    if(start < 0 || start >= arr->size)
    {
        return invalid_range;
    }
    if(count < 0 || start + count > arr->size)
    {
        return invalid_range;
    }
    int new_size = arr->size - count;
    if(new_size < 0)
    {
        return invalid_range;
    }
    if(new_size == 0)
    {
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        arr->buf = 0;
        return well;
    }
    int* tmp = (int*)malloc(sizeof(int) * new_size);
    if(tmp == NULL)
    {
        return mem_problem;
    }

    for(int i = 0; i < arr->size; ++i)
    {
        if(i >= start && i < start + count)
        {
            continue;
        }
        else if(i < start){
            tmp[i] = arr->data[i];
        }
        else{
            tmp[i - count] = arr->data[i];
        }
    }
    free(arr->data);
    arr->data = tmp;
    arr->size = new_size;

    return well;
}

state copy_part_arr(Massive* first, Massive* second, int start, int end)
{
    if(start < 0 || start >= first->size)
    {
        return invalid_range;
    }
    if(end < 0 || end >= first->size)
    {
        return invalid_range;
    }
    if(end < start)
    {
        return invalid_range;
    }


    free(second->data);
    second->data = NULL;
    second->data = (int*)malloc(sizeof(int) * (end - start + 1));
    if(second->data == NULL)
    {
        return mem_problem;
    }
    second->size = 0;
    second->buf = end - start + 1;

    for(int i = 0; i < end - start + 1; i++)
    {
        second->data[i] = first->data[start + i];
        second->size++;
    }

    return well;
}

int compare_plus(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int compare_minus(const void* a, const void* b)
{
    return (*(int*)b - *(int*)a);
}

state sort_array(Massive* arr, char type)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    int (*comp)(const void*, const void*);
    if(type == '-')
    {
        comp = compare_minus;
    }
    else if(type == '+')
    {
        comp = compare_plus;
    }
    else
    {
        return invalid_comand;
    }
    qsort(arr->data, arr->size, sizeof(int), comp);
    return well;

}

state shuffle(Massive* arr)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    for(int i = arr->size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp = arr->data[i];
        arr->data[i] = arr->data[j];
        arr->data[j] = tmp;
    }
    return well;
}

state get_max_array(Massive* arr, int* max, int* max_index)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    *max = arr->data[0];
    *max_index = 0;
    for(int i = 1; i < arr->size; i++)
    {
        if(arr->data[i] > *max)
        {
            *max = arr->data[i];
            *max_index = i;
        }
    }
    return well;
}

state get_min_array(Massive* arr, int* min, int* min_index)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    *min = arr->data[0];
    *min_index = 0;
    for(int i = 1; i < arr->size; i++)
    {
        if(arr->data[i] < *min)
        {
            *min = arr->data[i];
            *min_index = i;
        }
    }
    return well;
}

state get_most_often(Massive* arr, int* most_often, int* count)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    *most_often = arr->data[0];
    *count = 1;
    for(int i = 1; i < arr->size; i++)
    {
        int current_val = arr->data[i];
        int current_count = 1;
        for(int j = i + 1; j < arr->size; j++)
        {
            if(arr->data[j] == current_val)
            {
                current_count++;
            }
        }
        if(current_count > *count || (current_count == *count && current_val > *most_often))
        {
            *most_often = current_val;
            *count = current_count;
        }
    }
    return well;
}

state get_average_from_array(Massive* arr, double* average)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    *average = 0;
    for(int i = 0; i < arr->size; i++)
    {
        *average += arr->data[i];
    }
    *average /= (double )arr->size;
    return well;
}

state get_max_otklonenie(Massive* arr, double* max_otklonenie, int* position, int* number)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    double average;
    state st = get_average_from_array(arr, &average);
    if(st != well)
    {
        return st;
    }

    *max_otklonenie = 0;
    for(int i = 1; i < arr->size; i++)
    {
        double tmp = fabs(arr->data[i] - average);
        if(tmp > *max_otklonenie)
        {
            *max_otklonenie = tmp;
            *position = i;
            *number = arr->data[i];
        }
    }
    return well;
}

void print_stats_array(Massive* arr)
{
    int max, min, position, value, count;

    printf("Stats of array: %c\n", arr->name);
    printf("size of array: %d\n", arr->size);


    state st = get_max_array(arr, &max, &position);
    if(st != well)
    {
        print_status(st);
        return;
    }
    printf("max: %d, position: %d\n", max, position);

    st = get_min_array(arr, &min, &position);
    if(st != well)
    {
        print_status(st);
        return;
    }
    printf("min: %d, position: %d\n", min, position);

    st = get_most_often(arr, &value, &count);
    if(st != well)
    {
        print_status(st);
        return;
    }
    printf("most often: %d, count: %d\n", value, count);

    double average;
    st = get_average_from_array(arr, &average);
    if(st != well) {
        print_status(st);
        return;
    }
    printf("average: %f\n", average);

    double max_otklonenie;
    st = get_max_otklonenie(arr, &max_otklonenie, &position, &value);
    if(st != well) {
        print_status(st);
        return;
    }
    printf("max otklonenie: %f, position: %d, value: %d\n\n", max_otklonenie, position, value);
    print_all(arr);

}

state print_one_elem(Massive* arr, int index)
{
    if(index < 0 || index >= arr->size)
    {
        return invalid_index;
    }
    printf("ELem on position %d in array %c is %d\n", index, arr->name, arr->data[index]);
    return well;
}

state print_range(Massive* arr, int start, int end)
{
    if(start < 0 || start >= arr->size)
    {
        return invalid_index;
    }
    if(end < 0 || end >= arr->size)
    {
        return invalid_index;
    }
    if(start > end)
    {
        return invalid_range;
    }
    printf("Elements from %d to %d in array %c are: \n", start, end, arr->name);
    for(int i = start; i <= end; i++)
    {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return well;

}

state print_all(Massive* arr)
{
    if(arr->size == 0)
    {
        return empty_array;
    }
    printf("Elements in array %c are: \n", arr->name);
    for(int i = 0; i < arr->size; i++)
    {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return well;
}
