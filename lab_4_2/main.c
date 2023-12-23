#include "helper.h"


int main(int argc, char** argv) {
    if(argc != 2)
    {
        print_status(invalid_count_argc);
        return 0;
    }
    srand((unsigned int)time(NULL));

    state st = parse_file(argv[1]);
    if(st != well)
    {
        print_status(st);
        return 0;
    }
    print_status(well);
    return 0;

}

state parse_file(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        return probem_file_opening;
    }

    Massive* gen_array[26];
    for(int i = 0; i < 26; i++)
    {
        gen_array[i] = NULL;
    }
    int size = 0;
    int buf =  1;
    char* line = (char*)malloc(sizeof(char) * (buf+1));
    if(line == NULL)
    {
        fclose(file);
        return mem_problem;
    }
    state st;
    int ch;
    while((ch = getc(file)) != EOF)
    {
        //if(ch == '\n' || ch == '\r' || ch == '\t') continue;
        if(line == NULL || size >= buf - 1)
        {
            buf *= 2;
            char* tmp = realloc(line, sizeof(char) * (buf+1));
            if(tmp == NULL)
            {
                free(line);
                fclose(file);
                return mem_problem;
            }
            line = tmp;
        }
        line[size++] = (char)ch;

        if(ch == ';')
        {
            line[size] = '\0';
            size = 0;

            char* comand = strtok(line, " \n\t\r(");
            if(comand == NULL)
            {
                fclose(file);
                free(line);
                free_all_arrays(gen_array, 26);
                return invalid_format;
            }

            if(strcmp(comand, "Load") == 0)
            {
                char* array_name = strtok(NULL, " ,\n\t\r");
                if(array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_format;
                }

                char* filename_new = strtok(NULL, " ;\n\t\r");
                if(filename == NULL || strcmp(filename, filename_new) == 0)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return same_file;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                Massive* arr = NULL;
                int index = toupper(*array_name) - 'A';
                if(gen_array[index] != NULL)
                {
                    free_array(&(gen_array[index]));
                }

                if((st = create_new_array(&arr, *array_name, 1)) != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
                gen_array[index] = arr;

                if((st = get_arr_from_file(filename_new, arr)) != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }



            }
            else if(strcmp(comand, "Save") == 0)
            {
                char* arr_name = strtok(NULL, " \n\t\r,");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                char* filename_new = strtok(NULL, " ;\n\t\r");
                if(filename == NULL || strcmp(filename, filename_new) == 0)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                Massive* arr = NULL;
                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                st = save_to_file(filename_new, gen_array[index]);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }

            }
            else if(strcmp(comand, "Rand") == 0)
            {
                char* arr_name = strtok(NULL, " \n\t\r,");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int count;
                int lb;
                int rb;
                char* count_string = strtok(NULL, " ,\n\t\r");
                if(count_string == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                st = string_to_int(count_string, &count);
                if(st != well  || count <= 0)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    if(st == well)
                    {
                        return invalid_comand;
                    }
                    return st;
                }

                char* lb_string = strtok(NULL, " ,\n\t\r");
                if(lb_string == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                st = string_to_int(lb_string, &lb);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }

                char* rb_string = strtok(NULL, " ;\n\t\r");
                if(rb_string == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                st = string_to_int(rb_string, &rb);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {


                    st = create_new_array(&(gen_array[index]), *arr_name, 1);
                    if(st != well)
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }

                }
                else
                {
                    free_array(&(gen_array[index]));
                    st = create_new_array(&(gen_array[index]), *arr_name, 1);
                    if(st != well)
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }

                st = fill_randomly(gen_array[index], lb, rb, count);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }


            }
            else if(strcmp(comand, "Concat") == 0)
            {
                char* arr_name_first = strtok(NULL, " ,\n\t\r");
                if(arr_name_first == NULL || strlen(arr_name_first) != 1 || !isalpha(*arr_name_first))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                char* arr_name_second = strtok(NULL, " ;\n\t\r");
                if(arr_name_second == NULL || strlen(arr_name_second) != 1 || !isalpha(*arr_name_second))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int index_first = toupper(*arr_name_first) - 'A';
                if(gen_array[index_first] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int index_second = toupper(*arr_name_second) - 'A';
                if(gen_array[index_second] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                st = concatination_arrays(gen_array[index_first], gen_array[index_second]);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }


            }
            else if(strcmp(comand, "Free") == 0)
            {
                char* arr_name = strtok(NULL, ")");//возможно нужно вернуть (
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                free_array(&gen_array[index]);
            }
            else if(strcmp(comand, "Remove") == 0)
            {
                char* arr_name = strtok(NULL, " ,\n\t\r");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                char* start = strtok(NULL, " ,\n\t\r");
                char* count = strtok(NULL, " ,;\n\t\r");
                if(start == NULL || count == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int start_int;
                int count_int;
                st = string_to_int(start, &start_int);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                st = string_to_int(count, &count_int);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                st = remove_some_elem_from_array(gen_array[index], start_int, count_int);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }

            }
            else if(strcmp(comand, "Copy") == 0)
            {
                char* first_arr_name = strtok(NULL, " ,\n\t\r");
                if(first_arr_name == NULL || strlen(first_arr_name) != 1 || !isalpha(*first_arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                char* start = strtok(NULL, " ,\n\t\r");
                char* end = strtok(NULL, " ,\n\t\r");
                if(start == NULL || end == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                char* second_arr_name = strtok(NULL, " ;\n\t\r");
                if(second_arr_name == NULL || strlen(second_arr_name) != 1 || !isalpha(*second_arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                int index_first = toupper(*first_arr_name) - 'A';
                if(gen_array[index_first] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int index_second = toupper(*second_arr_name) - 'A';
                if(gen_array[index_second] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int start_int;
                int count_int;
                st = string_to_int(start, &start_int);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                st = string_to_int(end, &count_int);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                st = copy_part_arr(gen_array[index_first], gen_array[index_second], start_int, count_int);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }


            }
            else if(strcmp(comand, "Sort") == 0)
            {
                char* arr_name = strtok(NULL, " ;\n\t\r");//TODO убрать из разделителей + и - но считывать  только 2 символа strlen(arr_name) != 2
                if(arr_name == NULL || strlen(arr_name) != 2 || !isalpha(arr_name[0]) || (arr_name[1] != '+' && arr_name[1] != '-'))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                st = sort_array(gen_array[index], arr_name[1]);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }


            }
            else if(strcmp(comand, "Shuffle") == 0)
            {
                char* arr_name = strtok(NULL, " \n\t\r;");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                st = shuffle(gen_array[index]);
                if(st != well)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }


            }
            else if(strcmp(comand, "Stats") == 0)
            {
                char* arr_name = strtok(NULL, " ;\n\t\r");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }
                if(strtok(NULL, " ;\n\t\r") != NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                print_stats_array(gen_array[index]);


            }
            else if(strcmp(comand, "Print") == 0)
            {
                char* arr_name = strtok(NULL, " ,\n\t\r");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name))
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                char* next = strtok(NULL, " ,;\n\t\r");
                if(next == NULL)
                {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_comand;
                }

                char* next_next = strtok(NULL, " ;\n\t\r");

                if(strcmp(next, "all") == 0 && next_next == NULL)
                {
                    st = print_all(gen_array[index]);
                    if(st != well && st != empty_array)
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    if(st == empty_array)
                    {
                        print_status(empty_array);
                    }
                }
                else if(next_next != NULL)
                {
                    int start, end;
                    st = string_to_int(next, &start);
                    if(st != well )
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    st = string_to_int(next_next, &end);
                    if(st != well)
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    st = print_range(gen_array[index], start, end);
                    if(st != well ) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }
                else
                {
                    int current_position;
                    st = string_to_int(next, &current_position);
                    if(st != well)
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    st = print_one_elem(gen_array[index], current_position);
                    if(st != well)
                    {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }

            }
            else
            {
                fclose(file);
                free(line);
                free_all_arrays(gen_array, 26);
                return invalid_comand;
            }
        }
    }

    fclose(file);
    free(line);
    free_all_arrays(gen_array, 26);
    return well;
}













