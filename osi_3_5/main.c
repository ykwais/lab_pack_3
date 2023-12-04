#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define PATH 512

typedef enum status_code{
    well,
    problem_open_dir,
    problem_with_file,
    memory_problem
}state;

state solution(int argc, char** argv);

int main(int argc, char** argv) {
    if(argc < 2){
        printf("too little arguments!\n");
        return 0;
    }
    switch(solution(argc, argv)){
        case memory_problem:
            printf("problem with memory allocation!\n");
            break;
        case problem_open_dir:
            printf("problem with opening the directory!\n");
            break;
        case problem_with_file:
            printf("some problem with opening the file!\n");
            break;
        case well:
            break;
    }
}

state solution(int argc, char** argv)
{
    DIR* directory = NULL;
    struct dirent* file;

    for(int i = 1; i < argc; ++i){
        directory = opendir(argv[i]);
        if(directory == NULL){
            return problem_open_dir;
        }
        printf("Directory %d:\n", i);
        while((file = readdir(directory)) != NULL){
            char path[PATH];
            snprintf(path, sizeof(path), "%s/%s", argv[i], file->d_name);



            struct stat stat_file;
            if(stat(path, &stat_file) == 0){
                if(S_ISDIR(stat_file.st_mode)){
                    printf("%24s | DIRECTORY", file->d_name);
                }
                if(S_ISLNK(stat_file.st_mode)){

                    printf("%24s | LINK     ", file->d_name);
                }
                if(S_ISREG(stat_file.st_mode)){
                    printf("%24s | REGULAR  ", file->d_name);
                }
                printf("| first file's address of disk %ld\n", stat_file.st_dev);
            }else{

                closedir(directory);
                return problem_with_file;
            }

        }
        closedir(directory);
    }
    return well;
}