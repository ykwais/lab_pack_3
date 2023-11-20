#include "helper.h"

#define ADD 10


typedef struct{
    string* city;
    string* street;
    int number_house;
    string* corp;
    int number_appartment;
    string* index;
}address;

typedef struct{
    address getter;
    double weight;
    string* mail_id;
    string* creation_time;
    string* time_get;
}mail;

typedef struct{
    address* adress;
    mail* list_mails;
    int buff;
    int count_mail;

}post;

void interact(post** pst);
char* read_line(state* stat);
void print_status (state status);
void clear_input_buffer();
int read_and_convert_to_int(state* stat);
double read_and_convert_to_doub(state* stat);

int main(){

    post* pst = (post*)malloc(sizeof(post));
    if(pst == NULL){
        printf("no mem\n");
        return 0;
    }
    pst->list_mails = (mail*)malloc(sizeof(mail)*ADD);
    if(pst->list_mails == NULL){
        printf("no mem\n");
        free(pst);
        return 0;
    }
    pst->count_mail = 0;
    pst->buff = ADD;
    pst->adress = (address*) malloc(sizeof(address));
    if(pst->adress == NULL)
    {
        free(pst->list_mails);
        free(pst);
        printf("no memes\n");
        return 0;
    }
    pst->adress->index = NULL;
    pst->adress->city = NULL;
    pst->adress->corp = NULL;
    pst->adress->street = NULL;
    pst->adress->number_appartment = 0;
    pst->adress->number_house = 0;


    interact(&pst);

    free(pst->list_mails);
    free(pst->adress);
    free(pst);

}


void interact(post** pst)
{
    while(1)
    {
        printf("==================================================================\n");
        printf("Choose action:\n");
        printf("a - add mail\n");
        printf("r - remove mail\n");
        printf("s - search mail by id and full info about this mail\n");
        printf("f - find all delivered\n");
        printf("e - expired delivery time\n");
        printf("i - info about all mails by sorted\n");
        printf("q - exit\n");
        printf("?-: ");
        char act;
        fflush(stdin);
        scanf("%c", &act);
        int c;
        if((c = getchar()) != '\n' && c != EOF){
            clear_input_buffer();
            printf("you've inputted wrond action, rerun\n");
            continue;
        }


        state st = well;


        if(act == 'a'){
            mail new_mail;
            printf("a\n");
            printf("Input the main data to create mail:\n");
            printf("input the city of getter:\n");
            char* city = read_line(&st);
            if(st != well){
                print_status(st);
                break;
            }
            if(city[0] == '\0'){
                printf("city can't be empty!\n");
                free(city);
                continue;
            }
            new_mail.getter.city = create_string(city, &st);
            if(st != well){
                print_status(st);
                free(city);
                break;
            }

            printf("input the street of getter:\n");
            char* street = read_line(&st);
            if(st != well){
                print_status(st);
                free(city);
                free(new_mail.getter.city);
                break;
            }
            if(street[0] == '\0'){
                printf("street can't be empty!\n");
                free(city);
                free(street);
                free(new_mail.getter.city);
                continue;
            }
            new_mail.getter.street = create_string(street, &st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                break;
            }

            printf("input the house number of getter:\n");
            int num_house = read_and_convert_to_int(&st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                break;
            }
            if(num_house <= 0){
                printf("number of house can't be negative!\n");
                free(city);
                free(street);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                continue;
            }

            new_mail.getter.number_house = num_house;

            printf("Input the corp of getter:\n");
            char* corp = read_line(&st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                break;
            }
            if(corp[0] == '\0'){
                printf("street can't be empty!\n");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                continue;
            }
            new_mail.getter.corp = create_string(corp, &st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                break;
            }

            printf("input the appartment number of getter:\n");
            int num_app = read_and_convert_to_int(&st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                break;
            }
            if(num_app <= 0){
                printf("number of appartment can't be negative!\n");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                continue;
            }

            new_mail.getter.number_appartment = num_app;

            printf("input the index of getter:\n");
            char* index = read_line(&st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                break;
            }
            if(index[0] == '\0'){
                printf("index can't be empty!\n");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                continue;
            }
            if(strlen(index) != 6){
                printf("index must have 6 symbols of digit!\n");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                continue;
            }
            int q = 0;
            for(int i = 0; i < 6; ++i){
                if(!isdigit(index[i])){
                    q++;
                }
            }
            if(q != 0){
                printf("index must have 6 symbols of digit!\n");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                continue;
            }
            new_mail.getter.index = create_string(index, &st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                break;
            }

            printf("Input the weight of mail:\n");
            double mail_weight = read_and_convert_to_doub(&st);
            if(st != well){
                print_status(st);
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                free(new_mail.getter.index);
                break;
            }
            if(mail_weight < 0){
                printf("weight can't be negative!\n");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                free(new_mail.getter.index);
                continue;
            }
            if(fabs(mail_weight - 40) > 0.1){
                printf("We can deliver less than 40 kg.its too huge mail... sorry, we cant deliver this mail. Contact the shipping company\n ");
                free(city);
                free(street);
                free(corp);
                free(new_mail.getter.city);
                free(new_mail.getter.street);
                free(new_mail.getter.corp);
                free(new_mail.getter.index);
                continue;
            }

            new_mail.weight = mail_weight;



        }
        else if(act == 'r'){
            printf("r\n");
        }
        else if(act == 's'){
            printf("s\n");
        }
        else if(act == 'f'){
            printf("f\n");
        }
        else if(act == 'e'){
            printf("e\n");
        }
        else if(act == 'i'){
            printf("i\n");
        }
        else if(act == 'q'){
            printf("q\n");
            break;
        }
        else{
            printf("you've inputted wrond action, rerun\n");
        }



    }
}

char* read_line(state* stat)
{
    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*buf);
    if(result == NULL){
        *stat = meme_problem;
        return NULL;
    }
    int ch;
    while((ch = getchar()) != EOF && ch != '\n'){
        size++;
        if(size >= buf)
        {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*buf);
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
    *stat = well;
    return result;
}

void print_status (state status)
{
    switch (status)
    {
        case meme_problem:
            printf("Memory allocation problem\n");
            break;
        case invalid:
            printf("Error: Free null ptr\n");
            break;
        default:
            break;
    }
}


int read_and_convert_to_int(state* stat) {
    char line[256];
    if (!fgets(line, sizeof line, stdin)) {
        *stat = invalid;
        return 0;
    }
    clear_input_buffer();

    char *end;
    errno = 0;
    long val = strtol(line, &end, 10);

    if (errno == ERANGE || val > INT_MAX || val < INT_MIN || end == line || *end != '\n' && *end != '\0') {
        *stat = invalid;
        return 0;
    }
    *stat = well;
    return (int)val;
}

double read_and_convert_to_doub(state* stat) {
    char line[256];
    if (!fgets(line, sizeof line, stdin)) {
        *stat = invalid;
        return 0.0;
    }
    clear_input_buffer();

    char *end;
    errno = 0;
    double val = strtod(line, &end);

    if (errno == ERANGE ||  end == line || *end != '\n' && *end != '\0') {
        *stat = invalid;
        return 0.0;
    }
    *stat = well;
    return val;
}

void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

