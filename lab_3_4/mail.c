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
    bool is_deliv;
    string* time_deliv_dead;
    string* real_time_deliv;
}mail;



typedef struct{
    address* adress;
    mail* list_mails;
    int buff;
    int count_mail;

}post;

typedef enum status_id_mail{
    similar,
    not_similar,
    sim_invalid,
    sim_mem_problem
}state_id;



void interact(post** pst);
char* read_line(state* stat);
void print_status (state status);
void clear_input_buffer();
int read_and_convert_to_int(state* stat);
double read_and_convert_to_doub(state* stat);
char* get_current_time(state* stat);
char* get_time_delivery(mail* new_mail, state* stat);
state add_mail_to_post(post** pst, mail* cur_mail);
void print_mail(mail* ml);
void free_mail(mail ml);
state_id search_mail(post** pst, string* mail_id, int* index);
int compare_mail(const void* first, const void* second);
int compare_mail_for_find(const void* a, const void* b);
state search_delivered_mails(post** pst, mail** delivered, mail** expired, int* size1, int* size2);

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


    for(int i = 0; i < pst->count_mail; ++i){
        free_mail(pst->list_mails[i]);
    }
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
        printf("u - set the delivery time\n");
        printf("s - search mail by id and full info about this mail\n");
        printf("f - find all delivered and expired delivery time\n");
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
            free(city);

            printf("input the street of getter:\n");
            char* street = read_line(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                break;
            }
            if(street[0] == '\0'){
                printf("street can't be empty!\n");
                free(street);
                delete_string(new_mail.getter.city);
                continue;
            }
            new_mail.getter.street = create_string(street, &st);
            if(st != well){
                print_status(st);
                free(street);
                delete_string(new_mail.getter.city);
                break;
            }
            free(street);


            printf("input the house number of getter:\n");
            int num_house = read_and_convert_to_int(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                break;
            }
            if(num_house <= 0){
                printf("number of house can't be negative!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                continue;
            }

            new_mail.getter.number_house = num_house;

            printf("Input the corp of getter:\n");
            //TODO - if there no corp!!! либо удалить 190 строку
            char* corp = read_line(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                break;
            }
            if(corp[0] == '\0'){
                printf("corp can't be empty!\n");
                free(corp);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                continue;
            }
            new_mail.getter.corp = create_string(corp, &st);
            if(st != well){
                print_status(st);
                free(corp);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                break;
            }
            free(corp);

            printf("input the appartment number of getter:\n");
            int num_app = read_and_convert_to_int(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                break;
            }
            if(num_app <= 0){
                printf("number of appartment can't be negative!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                continue;
            }

            new_mail.getter.number_appartment = num_app;

            printf("input the index of getter:\n");
            char* index = read_line(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                break;
            }
            if(index[0] == '\0'){
                printf("index can't be empty!\n");
                free(index);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                continue;
            }
            if(strlen(index) != 6){
                printf("index must have 6 symbols of digit!\n");
                free(index);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
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
                free(index);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                continue;
            }
            new_mail.getter.index = create_string(index, &st);
            if(st != well){
                print_status(st);
                free(index);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                break;
            }
            free(index);

            printf("Input the weight of mail:\n");
            double mail_weight = read_and_convert_to_doub(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                break;
            }
            if(mail_weight < 0){
                printf("weight can't be negative!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                continue;
            }
            if(fabs(mail_weight - 40) < 0.1){
                printf("We can deliver more than 40 kg.its too huge mail... sorry, we cant deliver this mail. Contact the shipping company\n ");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                continue;
            }

            new_mail.weight = mail_weight;

            printf("Input the id of mail:\n");
            char* mail_id = read_line(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                break;
            }
            if(mail_id[0] == '\0'){
                printf("the id of mail can't be empty!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                continue;
            }
            if(strlen(mail_id) != 14){
                printf("the id of mail must have 14 symbols of digit!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                continue;
            }

            for(int i = 0; i < 14; ++i){
                if(!isdigit(mail_id[i])){
                    q++;
                }
            }
            if(q != 0){
                printf("the id of mail must have 14 symbols of digit!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                continue;
            }
            //TODO search mail_id ✓
            string* new_mail_id = create_string(mail_id, &st);
            if(st != well)
            {
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                break;
            }

            state_id st_id= search_mail(pst, new_mail_id, NULL);
            if(st_id == sim_invalid){
                st = invalid;
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == sim_mem_problem){
                st = meme_problem;
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == similar){
                printf("You have inputted existing id of mail!\n");
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                free(mail_id);
                delete_string(new_mail_id);
                continue;
            }

            new_mail.mail_id = new_mail_id;

            free(mail_id);

            char* create_time = get_current_time(&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                delete_string(new_mail.mail_id);
                break;
            }

            new_mail.creation_time = create_string(create_time, &st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                delete_string(new_mail.mail_id);
                break;
            }
            free(create_time);

            new_mail.is_deliv = false;
            new_mail.time_deliv_dead = NULL;
            new_mail.real_time_deliv = NULL;

            char* deliver_time_deadline = get_time_delivery(&new_mail,&st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                delete_string(new_mail.mail_id);
                delete_string(new_mail.creation_time);
                break;
            }

            new_mail.time_deliv_dead = create_string(deliver_time_deadline, &st);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                delete_string(new_mail.mail_id);
                delete_string(new_mail.creation_time);
                break;
            }
            free(deliver_time_deadline);



            st = add_mail_to_post(pst, &new_mail);
            if(st != well){
                print_status(st);
                delete_string(new_mail.getter.city);
                delete_string(new_mail.getter.street);
                delete_string(new_mail.getter.corp);
                delete_string(new_mail.getter.index);
                delete_string(new_mail.mail_id);
                delete_string(new_mail.creation_time);
                delete_string(new_mail.time_deliv_dead);
                break;
            }
            printf("well, your mail was successfully registrated!\n");
            printf("==================================================================================================================================\n");
            for(int i = 0; i < (*pst)->count_mail; ++i){
                print_mail(&(*pst)->list_mails[i]);
            }

        }
        else if(act == 'r'){
            printf("r\n");
            printf("Input the id of mail that you want to remove:\n");
            char* mail_id = read_line(&st);
            if(st != well){
                print_status(st);
                break;
            }
            if(mail_id[0] == '\0'){
                printf("the id of mail can't be empty!\n");
                free(mail_id);
                continue;
            }
            if(strlen(mail_id) != 14){
                printf("the id of mail must have 14 symbols of digit!\n");
                free(mail_id);
                continue;
            }
            int q = 0;
            for(int i = 0; i < 14; ++i){
                if(!isdigit(mail_id[i])){
                    q++;
                }
            }
            if(q != 0){
                printf("the id of mail must have 14 symbols of digit!\n");
                free(mail_id);
                continue;
            }
            string* new_mail_id = create_string(mail_id, &st);
            if(st != well)
            {
                print_status(st);
                free(mail_id);
                break;
            }

            int index;
            state_id st_id= search_mail(pst, new_mail_id, &index);
            if(st_id == sim_invalid){
                st = invalid;
                print_status(st);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == sim_mem_problem){
                st = meme_problem;
                print_status(st);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == not_similar){
                printf("You have inputted there no existing id of mail!\n");

                free(mail_id);
                delete_string(new_mail_id);
                continue;
            }




            if(!((*pst)->list_mails[index].is_deliv)){
                printf("this mail has not been delivered yet. You can't delete it from list of mails!\n");
                continue;
            }

            //free_mail((*pst)->list_mails[index]);
            for(int i = index; i < (*pst)->count_mail - 1; ++i)
            {
                (*pst)->list_mails[i] = (*pst)->list_mails[i+1];
            }

            free_mail((*pst)->list_mails[(*pst)->count_mail-1]);
            (*pst)->count_mail--;
            printf("mail: %s was removed!\n", new_mail_id->arg);
            free(mail_id);
            delete_string(new_mail_id);

            continue;



        }
        else if(act == 'u'){
            printf("u\n");
            printf("Input the id of mail that you want to set the delivery time:\n");
            char* mail_id = read_line(&st);
            if(st != well){
                print_status(st);
                break;
            }
            if(mail_id[0] == '\0'){
                printf("the id of mail can't be empty!\n");
                free(mail_id);
                continue;
            }
            if(strlen(mail_id) != 14){
                printf("the id of mail must have 14 symbols of digit!\n");
                free(mail_id);
                continue;
            }
            int q = 0;
            for(int i = 0; i < 14; ++i){
                if(!isdigit(mail_id[i])){
                    q++;
                }
            }
            if(q != 0){
                printf("the id of mail must have 14 symbols of digit!\n");
                free(mail_id);
                continue;
            }
            string* new_mail_id = create_string(mail_id, &st);
            if(st != well)
            {
                print_status(st);
                free(mail_id);
                break;
            }

            int index;
            state_id st_id= search_mail(pst, new_mail_id, &index);
            if(st_id == sim_invalid){
                st = invalid;
                print_status(st);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == sim_mem_problem){
                st = meme_problem;
                print_status(st);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == not_similar){
                printf("You have inputted there no existing id of mail!\n");

                free(mail_id);
                delete_string(new_mail_id);
                continue;
            }
            free(mail_id);
            delete_string(new_mail_id);

            if((*pst)->list_mails[index].is_deliv){
                printf("this mail has been delivered already. You can't delete it!\n");
                continue;
            }

            char* create_time = get_current_time(&st);
            if(st != well){
                print_status(st);
                break;
            }

            (*pst)->list_mails[index].real_time_deliv = create_string(create_time, &st);
            if(st != well){
                print_status(st);
                free(create_time);
                break;
            }
            (*pst)->list_mails[index].is_deliv = true;
            free(create_time);
            printf("your mail has been delivered!\n");
            continue;

        }
        else if(act == 's'){
            printf("s\n");
            printf("Input the id of mail that you want to get information about:\n");
            char* mail_id = read_line(&st);
            if(st != well){
                print_status(st);
                break;
            }
            if(mail_id[0] == '\0'){
                printf("the id of mail can't be empty!\n");
                free(mail_id);
                continue;
            }
            if(strlen(mail_id) != 14){
                printf("the id of mail must have 14 symbols of digit!\n");
                free(mail_id);
                continue;
            }
            int q = 0;
            for(int i = 0; i < 14; ++i){
                if(!isdigit(mail_id[i])){
                    q++;
                }
            }
            if(q != 0){
                printf("the id of mail must have 14 symbols of digit!\n");
                free(mail_id);
                continue;
            }
            string* new_mail_id = create_string(mail_id, &st);
            if(st != well)
            {
                print_status(st);
                free(mail_id);
                break;
            }

            int index;
            state_id st_id= search_mail(pst, new_mail_id, &index);
            if(st_id == sim_invalid){
                st = invalid;
                print_status(st);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == sim_mem_problem){
                st = meme_problem;
                print_status(st);
                free(mail_id);
                delete_string(new_mail_id);
                break;
            }

            if(st_id == not_similar){
                printf("You have inputted there no existing id of mail!\n");

                free(mail_id);
                delete_string(new_mail_id);
                continue;
            }
            free(mail_id);
            delete_string(new_mail_id);

            print_mail(&((*pst)->list_mails[index]));
            continue;
        }
        else if(act == 'f'){
            printf("f\n");
            mail* delivered = NULL;
            mail* expired = NULL;
            int size1 = 0;
            int size2 = 0;
            st = search_delivered_mails(pst, &delivered, &expired, &size1, &size2);
            if(st != well){
                print_status(st);
                break;
            }
            printf("Delivered mails:\n");
            if(size1 == 0){
                printf("there no such mails\n");
            }else{
                for(int i = 0; i < size1; ++i)
                {
                    print_mail(&delivered[i]);
                }
            }
//            for(int i = 0; i < size1; ++i)
//            {
//                free_mail(delivered[i]);
//            }
            free(delivered);

            printf("Expired mails:\n");
            if(size2 == 0){
                printf("there no such mails\n");
            }else{
                for(int i = 0; i < size2; ++i)
                {
                    print_mail(&expired[i]);
                }
            }
//            for(int i = 0; i < size2; ++i)
//            {
//                free_mail(expired[i]);
//            }
            free(expired);

        }
        else if(act == 'i'){
            printf("i\n");
            printf("the list of all mails:\n");
            if((*pst)->count_mail == 0){
                printf("there no mails\n");
            }else {
                for (int i = 0; i < (*pst)->count_mail; ++i) {
                    print_mail(&(*pst)->list_mails[i]);
                }
            }
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


char* get_current_time(state* stat)
{
    time_t current_time;
    struct tm *info;
    char *buffer = (char *)malloc(sizeof(char)*20); //формат "dd:MM:yyyy hh:mm:ss"

    if (buffer == NULL){
        *stat = meme_problem;
        return NULL;
    }


    time(&current_time);
    info = localtime(&current_time);

    if (info == NULL) {
        free(buffer);
        *stat = meme_problem;
        return NULL;
    }

    strftime(buffer, 20, "%d:%m:%Y %H:%M:%S", info);


    *stat = well;
    return buffer;
}

char* get_time_delivery(mail* new_mail, state* stat)
{
    time_t cur_time;
    struct tm* info_delivery;
    time(&cur_time);

    time_t how_long;
    if(new_mail->weight <= 2){
        how_long = 60;
    }else if(new_mail->weight > 2 && new_mail->weight < 7){
        how_long = 120;
    } else if(new_mail->weight >= 7 && new_mail->weight < 15 )
    {
        how_long = 180;
    }
    else{
        how_long = 240;
    }

    time_t time_finish = cur_time + how_long;
    info_delivery = localtime(&time_finish);
    if (info_delivery == NULL) {
        *stat = meme_problem;
        return NULL;
    }
    char *buffer = (char *)malloc(sizeof(char)*20); // 19 символов для формата "dd:MM:yyyy hh:mm:ss"

    if (buffer == NULL){
        *stat = meme_problem;
        return NULL;
    }

    strftime(buffer, 20, "%d:%m:%Y %H:%M:%S", info_delivery);
    //buffer[20] = '\0';

    *stat = well;
    return buffer;

}


state add_mail_to_post(post** pst, mail* cur_mail)
{
    if(pst == NULL || cur_mail == NULL)
    {
        return invalid;
    }
    if((*pst)->count_mail >= (*pst)->buff)
    {
        (*pst)->buff += ADD;
        mail* tmp = (mail*)realloc((*pst)->list_mails, sizeof(mail)*(*pst)->buff);
        if(tmp == NULL){
            return meme_problem;
        }else{
            (*pst)->list_mails = tmp;
        }
    }
    (*pst)->list_mails[(*pst)->count_mail] = *cur_mail;
    (*pst)->count_mail += 1;
    //TODO qsort ✓
    qsort((*pst)->list_mails, (*pst)->count_mail, sizeof(mail), compare_mail);
    return well;
}

void print_mail(mail* ml)
{
    printf("the city of getter: %s\n", ml->getter.city->arg != NULL ? ml->getter.city->arg : "UNKNOWN");
    printf("the street of getter: %s\n", ml->getter.street->arg != NULL ? ml->getter.street->arg : "UNKNOWN");
    printf("the house number of getter: %d\n", ml->getter.number_house);
    printf("the corp of getter: %s\n", ml->getter.corp->arg != NULL ? ml->getter.corp->arg : "UNKNOWN");
    printf("getter's number of appartment: %d\n", ml->getter.number_appartment);
    printf("the index of getter: %s\n", ml->getter.index->arg != NULL ? ml->getter.index->arg : "UNKNOWN");
    printf("the weqight of mail: %f\n", ml->weight);
    printf("the id of mail: %s\n", ml->mail_id->arg != NULL ? ml->mail_id->arg : "UNKNOWN");
    printf("the time of send: %s\n", ml->creation_time->arg != NULL ? ml->creation_time->arg : "UNKNOWN");
    printf("was it delivered?: %s\n", ml->is_deliv ? "YEE" : "NO(");
    printf("the deadline of delivery: %s\n", ml->time_deliv_dead->arg != NULL ? ml->time_deliv_dead->arg : "UNKNOWN");
    printf("the time of delivery: %s\n", ml->real_time_deliv != NULL ? ml->real_time_deliv->arg : "UNKNOWN");/////////////////////////////////TODO с учетом пустоты
    printf("==================================================================================================================================\n");
}


void free_mail(mail ml)
{
    delete_string(ml.getter.city);
    delete_string(ml.getter.street);
    delete_string(ml.getter.corp);
    delete_string(ml.getter.index);
    delete_string(ml.mail_id);
    delete_string(ml.creation_time);
    delete_string(ml.time_deliv_dead);
    delete_string(ml.real_time_deliv);
    //free(ml);
}

state_id search_mail(post** pst, string* mail_id, int* index)
{

    state st;
    for(int i = 0; i < (*pst)->count_mail; ++i)
    {
        if(!equal((*pst)->list_mails[i].mail_id, mail_id, &st)){
            if(st == invalid){
                return sim_invalid;
            }
            if(st == meme_problem){
                return sim_mem_problem;
            }
            if(index != NULL){
                *index = i;
            }
            return similar;
        }
    }
    return not_similar;
}

state search_delivered_mails(post** pst, mail** delivered, mail** expired, int* size1, int* size2)
{
    int full_count = (*pst)->count_mail;
    *delivered = (mail*)malloc(sizeof(mail)*full_count);
    if(*delivered == NULL)
    {
        return meme_problem;
    }
    *expired = (mail*)malloc(sizeof(mail)*full_count);
    if(*expired == NULL)
    {
        free(*delivered);
        return meme_problem;
    }

    int first_count = 0;
    int secound_count = 0;
    for(int i = 0; i < full_count; ++i)
    {
        if((*pst)->list_mails[i].is_deliv)
        {
            (*delivered)[first_count] = (*pst)->list_mails[i];
            first_count++;
        }else{
            state st = well;
            char* curr_time = get_current_time(&st);
            if(st != well){
//                for(int j = 0; j < first_count; ++j)
//                {
//                    //free_mail((*delivered)[j]);
//                    free(*delivered);
//                }
//                for(int j = 0; j < secound_count; ++j)
//                {
//                    //free_mail((*expired)[j]);
//                    free(*expired);
//                }
                return st;
            }
            if(strcmp((*pst)->list_mails[i].time_deliv_dead->arg, curr_time) < 0){
                (*expired)[secound_count] = (*pst)->list_mails[i];
                secound_count++;
            }
            free(curr_time);
        }

    }
    qsort(*delivered, first_count, sizeof(mail), compare_mail_for_find);
    qsort(*expired, secound_count, sizeof(mail), compare_mail_for_find);
    *size1 = first_count;
    *size2 = secound_count;
    return well;
}

int compare_mail(const void* first, const void* second)
{
    const mail* a = (const mail*)first;
    const mail* b = (const mail*)second;
    state st;
    int res = equal(a->getter.index, b->getter.index, &st);
    if(st != well)
    {
        return -1;
    }
    if(res != 0){
        return res;
    }

    res = equal(a->mail_id, b->mail_id, &st);
    if(st != well){
        return -1;
    }
    return res;

}

int compare_mail_for_find(const void* a, const void* b)
{
    const mail* mail_1 = (const mail*)a;
    const mail* mail_2 = (const mail*)b;

    return strcmp(mail_1->creation_time->arg, mail_2->creation_time->arg);
}


