#include <stdio.h>

typedef enum state{
    well,
    invalid,
    file_error
}state;

//input.txt flag number
//flag -o - oct(0 - 777)
// -d - dec (0 - 777(8))

state validete_input(int argc, char** argv);

int main(int argc, char** argv) {

}

state validete_input(int argc, char** argv){
    if(argc != 4){
        return invalid;
    }


}
