#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

typedef enum {
    OK = 1,
    FILE_ERROR,
    FLAG_ERROR,
    FORMAT_ERROR,
    BASE_ERROR,
}CODE_RESULT;

void status (CODE_RESULT flag){
    switch (flag) {
        case FILE_ERROR:
            printf("FILE ERROR!\n"); break;
        case FLAG_ERROR:
            printf("Wrong flag given! Possible flags: 'xor8', 'xor32', 'mask <hex>'\n<hex> is mask given in base 16\n"); break;
        case FORMAT_ERROR:
            printf("Wrong format!\n"); break;
        case BASE_ERROR:
            printf("Given mask isn't in base 16!\n"); break;
        case OK:
            printf("DONE!\n"); break;
    }
}

int range_checker(char* str, int base){
    int flag = 0;
    if (str[0] == '-'){
        flag = 1;
    }
    if (base <= 10){
        for (int i = 0; i < strlen(str) - flag; ++i) {
            if (str[i + flag] - '0' >= base){
                return 0;
            }
        }
    }
    else{
        for (int i = 0; i < strlen(str) - flag; ++i){
            if (!isdigit(str[i + flag]) && (str[i + flag] - 'A' >= base - 10)){
                return 0;
            }
        }
    }
    return 1;
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc < 3 || argc > 4){
        return FORMAT_ERROR;
    }
    char* flag = argv[2];
    if (strcmp(flag, "xor8") != 0 && strcmp(flag, "xor32") != 0 && strcmp(flag, "mask") != 0){
        return FLAG_ERROR;
    }
    if (strcmp(flag, "mask") == 0){
        if (argc != 4){
            return FORMAT_ERROR;
        }
        if (range_checker(argv[3], 16) == 0){
            return BASE_ERROR;
        }
        return OK;
    }
    return OK;
}

long int flag_xor8 (FILE *input){
    char c;
    long int res;
    while (fread(&c, sizeof(char), 1, input)) {
        res = res ^ c;
    }
    return res;
}

long int flag_xor32 (FILE *input){
    int32_t group1, group2, res = 0;
    int flag = 1;
    do {
        if (!fread(&group1, sizeof(int32_t), 1, input) || !fread(&group2, sizeof (int32_t), 1, input)){
            flag = 0;
        }
        int32_t temp = group1 ^ group2;
        res = res ^ temp;
    } while (flag == 1);
    return res;
}

long int flag_mask (FILE *input, char** argv){
    long int mask = strtol(argv[3], NULL, 16);
    long int res = 0;
    char c;
    while (fread(&c, sizeof(char), 1, input)){
        if ((mask | c) == mask){
            res++;
        }
    }
    return res;
}

CODE_RESULT flag_caller(char** argv){
    char* PATH = argv[1];
    FILE *input = fopen(PATH, "rb");
    if (input == NULL){
        return FILE_ERROR;
    }
    char* flag = argv[2];
    if (strcmp(flag, "xor32") == 0){
        printf("%ld\n", flag_xor32(input));
    }
    else if (strcmp(flag, "xor8") == 0){
        printf("%ld\n", flag_xor8(input));
    }
    else if(strcmp(flag, "mask") == 0){
        printf("%ld\n", flag_mask(input, argv));
    }
    fclose(input);
    return OK;
}

int main(int argc, char** argv){
    if (format_validation(argc, argv) == OK){
        status(flag_caller(argv));
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}