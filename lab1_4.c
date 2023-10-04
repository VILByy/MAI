#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define PATH "E:\123123a4343434f.txt"

typedef enum{
    OK = 1,
    WRONG_FLAG,
    NAN_GIVEN,
    WRONG_FORMAT,
    NO_FLAG,
    FILE_ERROR,
}CODE_RESULT;

CODE_RESULT format_validation(int argc, char **argv){
    if (argc < 3 || argc > 4){
        printf("Wrong format!");
        return WRONG_FORMAT;
    }
    char prefix = argv[1][0];
    if (prefix == '/' || prefix == '-'){
        if (strlen(argv[1]) == 3){
            char letter = argv[1][2];
            if (argv[1][1] == 'n' && (letter == 'd' || letter == 'i' || letter == 's' || letter == 'a') && argc == 4){
                return OK;
            }
            printf("Flag is not valid! Possible flags: -nd, -ni, -ns, -na, -d, -i, -s, -a.\n");
            return WRONG_FLAG;
        }
        if (strlen(argv[1]) == 2){
            char letter = argv[1][1];
            if ((letter == 'd' || letter == 'i' || letter == 's' || letter == 'a') && argc == 3){
                return OK;
            }
            printf("Flag is not valid! Possible flags: -nd, -ni, -ns, -na, -d, -i, -s, -a.\n");
            return WRONG_FLAG;
        }
    }
    printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
    return NO_FLAG;
}

CODE_RESULT flag_d(char **argv){
    FILE *fp;
    fp = fopen(PATH, "r");
    if (fp == NULL){
        printf("Error in opening file\n");
        return FILE_ERROR;
    }
    printf("FILE OPENED SUCCESSFULLY\n");
    return OK;
}

int main(int argc, char **argv){
     printf("%s\n", PATH);

}