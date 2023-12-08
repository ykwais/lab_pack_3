#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFF 200

typedef enum
{
    well,
    bad,
    invalid_input,
    eof,
    mem_problem,
    error_with_file
} state;

typedef struct Variable
{
    struct Variable *next;
    int value;
} Variable;

typedef struct{
    Variable *first;
    Variable *last;
    int degree;
}Polynomial;

state filter_read_line(FILE* file, char** line);
char** split_line(const char* line, const char* separate, int* amount);
state valid_polinom(char* str, char variable);
void free_arr(char** arr, int size);
state pre_build_polinom(char *line, int *pos, Polynomial **result, char variable);
state increase_polynom(Polynomial *p, int size);
Variable* search_by_degree(const Polynomial *p, int index);
state summ_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result);
void free_elem(Variable *current);
state free_polinomial(Polynomial *p);
state build_Polynomial(char *input, int n, Polynomial **a, char variable);
state multy_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result);
state sub_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result);
state do_command(char* line, Polynomial** p, char variable, long long** tmp, int** current);
state calculate_polinom(const Polynomial* pol, long long* res, int num);
state diff_polinom(Polynomial* p);
state division_polinom(const Polynomial* first, const Polynomial* second, Polynomial** major, Polynomial** minor);
state div_polynom(const Polynomial *a, const Polynomial *b, Polynomial **major);
state mod_polynom(const Polynomial *a, const Polynomial *b, Polynomial **minor);
state cmp_polinom(const Polynomial* first, const Polynomial* second, Polynomial** res);
void print_polynom(Polynomial *p, char variable);
state task(const char * file_name, char variable);
void print_status_codes(state code);

state create_polynom(Polynomial** p)
{
    Polynomial* tmp = (Polynomial*)malloc(sizeof(Polynomial));
    if(tmp == NULL){
        return mem_problem;
    }
    tmp->degree = 0;
    tmp->first = (Variable*)malloc(sizeof(Variable));
    if(tmp->first == NULL){
        free(tmp);
        return mem_problem;
    }
    tmp->last = tmp->first;
    tmp->first->next = NULL;
    *p = tmp;
    return well;
}

state add_coefs(Polynomial* p, int coef)
{
    if(p->degree == 0)
    {
        p->degree++;
        p->first->value = coef;
        return well;
    }
    Variable* tmp = p->last;
    tmp->next = (Variable*)malloc(sizeof(Variable));
    if(tmp->next == NULL){
        return mem_problem;
    }
    tmp->next->next = NULL;
    p->last = tmp->next;
    p->last->value = coef;
    p->degree++;
    return well;
}

state init_polynom(Polynomial** p, int value)
{
    state st = create_polynom(p);
    if(st != well){
        return st;
    }
    st = add_coefs(*p, value);
    if(st != well){
        free_polinomial(*p);
        return st;
    }
    return well;

}

int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("Error: wrong number of arguments\n");
    }
    if(strlen(argv[2]) != 1)
    {
        printf("Error: wrong variable\n");
    }

    state st = task(argv[1], argv[2][0]);
    print_status_codes(st);
}
state task(const char * file_name, char variable)
{
    FILE* file = fopen(file_name, "r");
    if(file == NULL){
        return error_with_file;
    }
    state st = well;
    Polynomial* p;
    Polynomial* new;

    st = init_polynom(&p, 0);
    if(st != well){
        return st;
    }
    char* command = NULL;

    while((st = filter_read_line(file, &command)) == well) {
        long long *temp = NULL;
        int *crnt_variable = NULL;

        st = do_command(command, &p, variable, &temp, &crnt_variable);
        if(st != well){
            free(command);
            free_polinomial(p);
            fclose(file);
            return st;
        }
        free(command);
        command = NULL;
        if(temp == NULL)
        {
            print_polynom(p, variable);
        }
        else{
            printf("Polynomial eval with %c = %d is : %lld\n", variable, *crnt_variable, *temp);
            free(temp);
        }

    }
    if(command != NULL) free(command);
    free_polinomial(p);
    fclose(file);

    return well;
}

state filter_read_line(FILE* file, char** line)
{
    char* line_of_file = (char*)malloc(sizeof(char)*BUFF);
    if(line_of_file == NULL){
        return mem_problem;
    }
    int buf = BUFF;
    int size = 0;
    bool is_one_line_comment = false;
    bool is_big_line_comment = false;
    int ch;

    while((ch = fgetc(file)) != EOF && !(ch == ';' && !is_one_line_comment && !is_big_line_comment)){
        if(buf - 1 <= size){
            buf *= 2;
            char* new_line = (char*)realloc(line_of_file, sizeof(char)*buf);
            if(new_line == NULL){
                free(line_of_file);
                return mem_problem;
            }
            line_of_file = new_line;

        }
        if(!is_big_line_comment && ch == '%') is_one_line_comment = true;
        if(!is_one_line_comment && ch == '[') is_big_line_comment = true;
        if(ch != '\n' && ch != ' ' && ch != '\t' && !is_one_line_comment && !is_big_line_comment){
            line_of_file[size] = (char)ch;
            size++;
        }
        if(ch == '\n' && is_one_line_comment) is_one_line_comment = false;
        if(is_big_line_comment && ch == ']') is_big_line_comment = false;
    }
    line_of_file[size] = '\0';
    *line = line_of_file;
    if(ch == EOF) return eof;
    return well;
}

int count_symb(const char* str, char symb)
{
    int count = 0;
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == symb) count++;
    }
    return count;
}

state check_line(char* line, char variable)
{
    int len = (int) strlen(line);
    if(line[len-1]!= ')' || count_symb(line, ',') > 1)
    {
        return invalid_input;
    }
    for(int i = 0; i < len-1; i++){
        if(!isdigit(line[i]) && !isalpha(line[i]) && line[i] != '(' && line[i] != ',' && line[i] != '^' && line[i] != variable && line[i] != '+' && line[i] != '-' ){
            return invalid_input;
        }
    }
    return well;
}

void swap_polin(Polynomial** f, Polynomial** s)
{
    Polynomial** tmp = f;
    f = s;
    s = tmp;
}

state do_command(char* line, Polynomial** p, char variable, long long** value, int** current)
{
    int len = (int)strlen(line);
    int count1 = count_symb(line, '(');
    int count2 = count_symb(line, ')');

    if(count1 != count2 || count1 != 1 || check_line(line, variable) != well){
        return invalid_input;
    }

    if(strncmp(line, "Mult(", 5 ) != 0 && strncmp(line, "Add(", 4 ) != 0 &&
       strncmp(line, "Sub(", 4 ) != 0 && strncmp(line, "Div(", 4 ) != 0
        && strncmp(line, "Mod(", 4) != 0 && strncmp(line, "Diff(", 5) !=0
        && strncmp(line, "Cmps(", 5) !=0 && strncmp(line, "Eval(", 5) !=0){
        return invalid_input;
    }

    char* separate = "()";
    int amount;
    char** tokens = split_line(line, separate, &amount);
    if(tokens == NULL){
        return mem_problem;
    }
    if(!(amount > 1 && tokens[1] != NULL && strlen(tokens[1]) > 0))
    {
        for(int i = 0; i < amount; i++){
            if(tokens[i] != NULL){
                free(tokens[i]);
            }
        }
        free(tokens);
        return bad;
    }

    char* pols = tokens[1];
    int n;
    char** input = split_line(pols, ",", &n);
    if(input == NULL){
        for(int i = 0; i < amount; i++){
            if(tokens[i] != NULL){
                free(tokens[i]);
            }
        }
        free(tokens);
        return mem_problem;
    }


    for(int i = 0; i < amount; i++){
        if(tokens[i] != NULL){
            free(tokens[i]);
        }
    }
    free(tokens);

    if(n < 1)
    {
        free_arr(input, n);
        return invalid_input;
    }

    for(int i = 0; i<n; ++i){
        state stat = valid_polinom(input[i], variable);
        if(stat != well){
            free_arr(input, n);
            return stat;
        }
    }

    if(strncmp(line, "Eval", 4) == 0 && n > 0)
    {
        Polynomial* pol;
        int k = 0;
        if(n == 2)
        {
            free_polinomial(*p);
            state stat = build_Polynomial(input[k++], n, &pol, variable);
            if(stat != well){
                free_polinomial(pol);
                free_arr(input, n);
                return stat;
            }
        }
        else pol = *p;
        long long val;
        int num;
        char* end;
        num = strtol(input[k], &end, 10);
        if(end == input[k] || *end != '\0'){
            free_polinomial(pol);
            free_arr(input, n);
            return invalid_input;
        }

        state st = calculate_polinom(pol, &val, num);
        if(st != well){
            free_polinomial(pol);
            free_arr(input, n);
            return st;
        }
        printf("calculated polynom: %lld\n", val);
        *value = (long long*)malloc(sizeof(int));
        if(*value == NULL){
            free_polinomial(pol);
            free_arr(input, n);
            return mem_problem;
        }
        *current = (int*)malloc(sizeof(int));
        if(*current == NULL){
            free(*value);
            free_polinomial(pol);
            free_arr(input, n);
            return mem_problem;
        }
        **value = val;
        **current = num;
        *p = pol;
        free_arr(input, n);
        return well;
    }
    else if(strncmp(line, "Diff", 4) == 0)
    {
        Polynomial* pol;
        if(n == 1)
        {
            state st = build_Polynomial(input[0], n, &pol, variable);
            free_polinomial(*p);
            if(st != well){
                free_polinomial(pol);
                free_arr(input, n);
                return st;
            }
        }
        else pol = *p;

        state st = diff_polinom(pol);
        if(st != well){
            free_polinomial(pol);
            free_arr(input, n);
            return st;
        }
        *p = pol;
    }

    Polynomial* p1 = NULL;
    Polynomial* p2 = NULL;

    state st = build_Polynomial(input[0], n, &p1, variable);
    if(st != well){
        free_polinomial(p1);
        free_polinomial(p2);
        free_arr(input, n);
        return st;
    }
    if(n == 2)
    {
        st = build_Polynomial(input[1], n, &p2, variable);
        free_polinomial(*p);
        if(st != well){
            free_polinomial(p1);
            free_polinomial(p2);
            free_arr(input, n);
            return st;
        }
    }
    else
    {
        p2 = *p;
        swap_polin(&p1, &p2);
    }

    if(strncmp(line, "Mult", 4) == 0 && n > 0)
    {

        st = multy_polinom(p1, p2, p);
        free_polinomial(p1);
        free_polinomial(p2);
        if(st != well){
            free_arr(input, n);
            return st;
        }
        free_arr(input, n);
        return well;

    }
    else if(strncmp(line, "Add", 3) == 0 && n > 0)
    {
        st = summ_polinom(p1, p2, p);
        free_polinomial(p1);
        free_polinomial(p2);
        if(st != well){
            free_arr(input, n);
            return st;
        }
        free_arr(input, n);
        return well;
    }
    else if(strncmp(line, "Sub", 3) == 0 && n > 0)
    {
        st = sub_polinom(p1, p2, p);
        free_polinomial(p1);
        free_polinomial(p2);
        if(st != well){
            free_arr(input, n);
            return st;
        }
        free_arr(input, n);
        return well;
    }

    else if(strncmp(line, "Div", 3) == 0 && n > 0)
    {
        st = div_polynom(p1, p2, p);
        free_polinomial(p1);
        free_polinomial(p2);
        if(st != well){
            free_arr(input, n);
            return st;
        }
        free_arr(input, n);
        return well;
    }
    else if(strncmp(line, "Mod", 3) == 0 && n > 0) {
        Polynomial* pol3;
        st = mod_polynom(p1, p2, &pol3);
        free_polinomial(p1);
        free_polinomial(p2);
        if (st != well) {
            free_arr(input, n);
            return st;
        }
        free_arr(input,n);
        return well;
    }
    else if(strncmp(line, "Cmps", 4) == 0 && n > 0)
    {
        st = cmp_polinom(p1, p2, p);
        free_polinomial(p1);
        free_polinomial(p2);
        if(st != well){
            free_arr(input, n);
            return st;
        }
        free_arr(input, n);
        return well;
    }
    else{
        free_polinomial(p1);
        free_polinomial(p2);
        free_arr(input, n);
        return invalid_input;
    }

}

void free_arr(char** arr, int size)
{
    for(int i = 0; i < size; i++){
        free(arr[i]);
    }
    free(arr);
}

char** split_line(const char* line, const char* separate, int* amount)
{
    int size = 0;
    int buf = 2;
    char** tokens = (char**)malloc(sizeof(char*)*buf);
    if(tokens == NULL){
        return NULL;
    }
    char* token = strtok((char*)line, separate);
    while(token != NULL){
        if(size >= buf-1){
            buf *= 2;
            char** new_tokens = (char**)realloc(tokens, sizeof(char*)*buf);
            if(new_tokens == NULL){
                for(int i = 0; i < size; i++){
                    free(tokens[i]);
                }
                free(tokens);
                return NULL;
            }
            tokens = new_tokens;
        }

        tokens[size] = (char*)malloc(sizeof(char)*(strlen(token)+1));
        if(tokens[size] == NULL){
            for(int i = 0; i < size; i++){
                free(tokens[i]);
            }
            free(tokens);
            return NULL;
        }

        strcpy(tokens[size], token);
        size++;
        token = strtok(NULL, separate);
    }
    tokens[size] = NULL;
    *amount = size;
    return tokens;
}


state valid_polinom(char* str, char variable)
{
    int len = (int)strlen(str);
    if(len == 0){
        return invalid_input;
    }

    bool flag = (str[0] == '+' || str[0] == '-')? true : false;
    bool exp = true;
    int i = 0;
    while(i < len)
    {
        if((str[i] == '+' || str[i] == '-') && flag){
            i++;
            flag = false;
            exp = true;
        }
        else if(str[i] == variable && exp)
        {
            i++;
            if(i < len && str[i]== '^'){
                i++;
                if(i < len && isdigit(str[i]))
                {
                    while(i < len && isdigit(str[i])){
                        i++;
                    }
                }
                else{
                    return invalid_input;
                }
            }
            exp = false;
            flag = true;
        }
        else if(isdigit(str[i]) && !flag)
        {
            while(i < len && isdigit(str[i])){
                i++;
            }
            flag = true;
        }
        else{
            return invalid_input;
        }
    }
    return flag ? well : invalid_input;

}

void free_elem(Variable *current)
{
    if (current == NULL)
    {
        return;
    }
    if (current->next != NULL)
    {
        free_elem(current->next);
    }
    free(current);

}

state free_polinomial(Polynomial *p)
{
    if (p == NULL) {return invalid_input; }
    if (p->first != NULL) {
        free_elem(p->first);
    }
    free(p);
    p = NULL;
    return well;
}

state build_Polynomial(char *input, int n, Polynomial **a, char variable)
{
    int l = 0;
    Polynomial *p = NULL, *current = NULL, *temper = NULL;
    state status = init_polynom(&current, 0);
    if (status != well) return mem_problem;
    int length = (int)strlen(input);

    while (l < length)
    {
        status = pre_build_polinom(input, &l, &p, variable);
        if (status != well)
        {
            free_polinomial(p);
            free_polinomial(current);
            return status;
        }
        status = summ_polinom(current, p, &temper);
        if (status != well)
        {
            free_polinomial(p);
            free_polinomial(current);
            free_polinomial(temper);
            return status;
        }
        free_polinomial(current);
        free_polinomial(p);
        p = NULL;
        current = temper;
    }
    *a = current;
    return well;
}



state pre_build_polinom(char *line, int *pos, Polynomial **result, char variable)
{
    Polynomial *ptr;
    int length = (int)strlen(line);
    int coeff = -1;
    int i = *pos;

    int sign = 1;
    if (line[i] == '+' || line[i] == '-')
    {
        sign = line[i] == '-' ? -1 : 1;
        i++;
    }

    while (i < length && isdigit(line[i]) != 0)
    {
        if (coeff == -1) coeff = 0;
        coeff *= 10;
        coeff += (line[i] - '0');
        i++;
    }
    if (coeff == -1) coeff = 1;

    coeff *= sign;
    if (i >= length || line[i] != variable)
    {
        state status = create_polynom(&ptr);
        if (status != well)
        {
            return status;
        }
        status = add_coefs(ptr, coeff);
        if (status != well)
        {
            free_polinomial(ptr);
            return status;
        }
        *result = ptr;
        *pos = i;
        return well;
    }
    i++;

    if (i >= length || line[i] != '^')
    {
        state status = create_polynom(&ptr);
        if (status != well)
        {
            return status;
        }
        status = add_coefs(ptr, coeff);
        if (status != well)
        {
            free_polinomial(ptr);
            return status;
        }
        status = add_coefs(ptr, 0);
        if (status != well)
        {
            free_polinomial(ptr);
            return status;
        }
        *result = ptr;
        *pos = i;
        return well;
    }
    i++;
    state status = create_polynom(&ptr);
    if (status != well) return status;

    int power = -1;
    while (i < length && isdigit(line[i]) != 0)
    {
        if (power == -1) power += 1;
        power *= 10;
        power += (line[i] - '0');
        i++;
    }
    status = add_coefs(ptr, coeff);
    if (status != well)
    {
        free_polinomial(ptr);
        return status;
    }
    while (power > 0)
    {
        power--;
        status = add_coefs(ptr, 0);
        if (status != well) return status;
    }
    *result = ptr;
    *pos = i;
    return well;
}

state increase_polynom(Polynomial *p, int size)
{
    if (size < p->degree ) {return invalid_input; }
    while (size > p->degree)
    {
        state status = add_coefs(p, 0);
        if (status != well) {return status;}
    }
    return well;
}

Variable* search_by_degree(const Polynomial *p, int index)
{
    Variable *ptr = p->first;
    int i = 0;
    while (i < index)
    {
        ptr = ptr->next;
        i++;
    }
    return ptr;
}

state summ_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result)
{
    Polynomial* p;
    state st = create_polynom(&p);
    if (st != well) return st;
    int degree = (first->degree > second->degree) ? first->degree : second->degree;
    st = increase_polynom(p, degree);
    if (st != well)
    {
        free_polinomial(p);
        return st;
    }

    for(int i = first->degree; i > 0; i--)
    {
        Variable* in = search_by_degree(p, degree - i);
        Variable* f = search_by_degree(first, first->degree - i);
        in->value = f->value;
    }

    for(int i = second->degree; i > 0; i--)
    {
        Variable* in = search_by_degree(p, degree - i);
        Variable* s = search_by_degree(second, second->degree - i);
        in->value += s->value;
    }

    *result = p;
    return well;
}

state sub_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result) {
    Polynomial *p;
    state st = create_polynom(&p);
    if (st != well) return st;
    int degree = (first->degree > second->degree) ? first->degree : second->degree;
    st = increase_polynom(p, degree);
    if (st != well) {
        free_polinomial(p);
        return st;
    }

    for(int i = first->degree; i > 0; i--)
    {
        Variable* in = search_by_degree(p, degree - i);
        Variable* f = search_by_degree(first, first->degree - i);
        in->value = f->value;
    }

    for(int i = second->degree; i > 0; i--)
    {
        Variable* in = search_by_degree(p, degree - i);
        Variable* s = search_by_degree(second, second->degree - i);
        in->value -= s->value;
    }

    *result = p;
    return well;

}

state multy_polinom(const Polynomial* first, const Polynomial* second, Polynomial** result)
{
    Polynomial* p;
    state st = create_polynom(&p);
    if (st != well) return st;
    int degree = first->degree + second->degree - 1;// 2 == 3, 3 == 4 => 2+3 = 3 + 4 -1
    st = increase_polynom(p, degree);
    if (st != well)
    {
        free_polinomial(p);
        return st;
    }

    for(int i = 0; i < first->degree; i++)
    {
        Variable* in = search_by_degree(first, i);
        for(int j = 0; j < second->degree; j++)
        {
            Variable* s = search_by_degree(second, j);
            Variable* out = search_by_degree(p, i + j);
            out->value += in->value * s->value;
        }
    }

    *result = p;
    return well;
}

state calculate_polinom(const Polynomial* pol, long long* res, int num)
{
    if(pol == NULL) return invalid_input;
    *res = 0;
    Variable* ptr = pol->first;
    for(int i = 0; i < pol->degree; i++)
    {
        if(ptr == NULL) break;
        *res *= num;
        *res += ptr->value;
        ptr = ptr->next;
    }
    return well;
}

state diff_polinom(Polynomial* p)
{
    if(p == NULL) return invalid_input;
    if(p->degree == 1)
    {
        p->first->value = 0;
        return well;
    }

    Variable* ptr = p->first;
    for(int i = 0; i < p->degree -1; i++)
    {
        ptr->value = ptr->value * ((p->degree) - i - 1);
        ptr = ptr->next;
    }

    Variable* a = search_by_degree(p, p->degree - 2);
    free_elem(ptr);
    p->degree--;
    a->next = NULL;

    return well;
}

state division_polinom(const Polynomial* first, const Polynomial* second, Polynomial** major, Polynomial** minor) {
    Polynomial *_major;
    Polynomial *_minor;
    state st = create_polynom(&_major);
    if (st != well) return st;
    st = create_polynom(&_minor);
    if (st != well) {
        free_polinomial(_major);
        return st;
    }
    int size = first->degree;
    st = increase_polynom(_major, size);
    if (st != well) {
        free_polinomial(_major);
        free_polinomial(_minor);
        return st;
    }
    st = increase_polynom(_minor, size);
    if (st != well) {
        free_polinomial(_major);
        free_polinomial(_minor);
        return st;
    }

    if (first->degree < second->degree) return invalid_input;///////

    Variable *ptr = first->first;
    Variable *ptr2 = second->first;

    for (int i = 0; i < first->degree; i++) {
        ptr2->value = ptr->value;
        ptr = ptr->next;
        ptr2 = ptr2->next;
    }
    ptr = _minor->first;
    ptr2 = _major->first;

    for (int i = 0; i <= (first->degree - second->degree); i++) {
        if (second->first->value == 0) {
            free_polinomial(_major);
            free_polinomial(_minor);
            return invalid_input;
        }

        int coef = (int) (ptr->value / second->first->value);
        ptr2->value = coef;

        Variable *ptr3 = second->first;
        for (int j = 0; j < second->degree; j++) {
            Variable *ptr4 = search_by_degree(_minor, i + j);
            ptr4->value -= ptr3->value * coef;
            ptr3 = ptr3->next;
        }
        ptr = ptr->next;
        ptr2 = ptr2->next;

    }

    ptr = _minor->first;
    Variable *end = _minor->first;
    int degree = 0;
    for (int i = 0; ptr != NULL; ptr = ptr->next) {
        if (ptr->value != 0) {
            end = ptr;
            degree++;
        }
        i++;
    }

    _minor->degree = degree;
    free_elem(end->next);
    end->next = NULL;

    if (_minor->degree == 0) {
        free_polinomial(_minor);
        st = create_polynom(&_minor);
        if (st != well) {
            free_polinomial(_major);
            return st;
        }

        st = increase_polynom(_minor, 1);
        if (st != well) {
            free_polinomial(_major);
            free_polinomial(_minor);
            return st;
        }
        _minor->degree = 1;

    }

    degree = 0;
    ptr = _major->first;
    end = _major->first;
    for (int i = 0; ptr != NULL; ptr = ptr->next) {
        if (ptr->value != 0) {
            end = ptr;
            degree++;
        }
        i++;
    }

    _major->degree = degree;
    free_elem(end->next);
    end->next = NULL;

    if (_major->degree == 0) {
        free_polinomial(_major);
        st = create_polynom(&_major);
        if (st != well) {
            free_polinomial(_minor);
            return st;
        }

        st = increase_polynom(_major, 1);
        if (st != well) {
            free_polinomial(_major);
            free_polinomial(_minor);
            return st;
        }
        _major->degree = 1;

    }
    *major = _major;
    *minor = _minor;
    return well;
}


state div_polynom(const Polynomial *a, const Polynomial *b, Polynomial **major)
{
    Polynomial *temp = NULL;
    state status = division_polinom(a, b, major, &temp);
    if (status != well) { return status ;}
    free_polinomial(temp);
    return well;
}

state mod_polynom(const Polynomial *a, const Polynomial *b, Polynomial **minor)
{
    Polynomial *temp = NULL;
    state status = division_polinom(a, b, &temp, minor);
    if (status != well) { return status ;}
    free_polinomial(temp);
    return well;
}

state multiply_polynomial_by_scalar(Polynomial *a, int scalar)
{
    Variable *ptrA = a->first;
    for (int i = 0; i < a->degree; i++)
    {
        ptrA->value *= scalar;
        ptrA = ptrA->next;
    }

    return well;
}

state cmp_polinom(const Polynomial* first, const Polynomial* second, Polynomial** res)
{
    Polynomial* result;
    state st = create_polynom(&result);
    if (st != well) return st;

    Variable* ptr_f = first->first;
    int degree = first->degree - 1;

    while(ptr_f != NULL)
    {
        Polynomial* tmp = NULL;
        st = init_polynom(&tmp, 1);
        if (st != well) {
            free_polinomial(result);
            return st;
        }

        while ((degree--) > 0)
        {
            st = multy_polinom(tmp, second, &tmp);
            if (st != well) {
                free_polinomial(result);
                free_polinomial(tmp);
                return st;
            }
        }
        multiply_polynomial_by_scalar(tmp, ptr_f->value);
        st = summ_polinom(result, tmp, &result);
        if (st != well) {
            free_polinomial(result);
            free_polinomial(tmp);
            return st;
        }
        ptr_f = ptr_f->next;
        free_polinomial(tmp);
    }

    *res = result;
    return well;
}

void print_elements(Variable *e, char variable, int degree)
{
    if (e == NULL) {return; }
    printf("%d*%c^%d", e->value, variable, degree);
    if (e->next != NULL)
    {
        printf(" + ");
        print_elements(e->next, variable, degree - 1);
    }
}

void print_polynom(Polynomial *p, char variable)
{
    if (p == NULL) { return; }
    printf("Degree of current polynomial is %d : ", p->degree - 1);
    print_elements(p->first, variable, p->degree - 1);
    printf("\n");
}


void print_status_codes(state code)
{
    switch (code)
    {
        case well:
            printf("Success\n");
            break;
        case bad:
            printf("Not success\n");
            break;
        case invalid_input:
            printf("Invalid input\n");
            break;
        case eof:
            printf("End of file\n");
            break;
        case mem_problem:
            printf("Memory allocation problem\n");
            break;
        case error_with_file:
            printf("Error with file\n");
            break;
        default:
            printf("Unknown status code\n");
            break;
    }
}