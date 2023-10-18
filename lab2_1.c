#include <stdio.h>
#include <ctype.h>
#include "stdlib.h"

typedef enum {
    OK = 1,
    WRONG_FORMAT,
    WRONG_FLAG,
    NAN_GIVEN,
} CODE_RESULT;

void status(CODE_RESULT flag) {
    switch (flag) {
        case WRONG_FORMAT:
            printf("Wrong format!\n");
            break;
        case OK:
            printf("Done!\n");
            break;
        case WRONG_FLAG:
            printf("Wrong flag given! Possible flags: -l, -r, -u, -n, -c.\n");
            break;
        case NAN_GIVEN:
            printf("Not a number given!\n");
            break;
    }
}

int str_len(char const *str) {
    int i = 0, res = 0;
    char c = str[0];
    while (c != '\0') {
        res++;
        c = str[i];
        i++;
    }
    return res;
}

int range_checker(char *str, int base) {
    int flag = 0;
    if (str[0] == '-') {
        flag = 1;
    }
    if (base <= 10) {
        for (int i = 0; i < str_len(str) - flag; ++i) {
            if (str[i + flag] - '0' >= base) {
                return 0;
            }
        }
    } else {
        for (int i = 0; i < str_len(str) - flag; ++i) {
            if (!isdigit(str[i + flag]) && (str[i + flag] - 'A' >= base - 10)) {
                return 0;
            }
        }
    }
    return 1;
}

CODE_RESULT format_validation(int argc, char **argv) {
    if (argc < 3) {
        return WRONG_FORMAT;
    }
    char flag = argv[1][1];
    if (flag == 'l' || flag == 'r' || flag == 'u' || flag == 'n') {
        if (argc == 3) {
            return OK;
        }
        return WRONG_FORMAT;
    } else if (flag == 'c') {
        if (argc < 5) {
            return WRONG_FORMAT;
        }
        if (range_checker(argv[3], 10) == 1) {
            return OK;
        }
        return NAN_GIVEN;
    }
    return WRONG_FLAG;
}

int flag_l(char *str){
    return str_len(str) - 1; // str_len counts \0
}

char* flag_r(char *str){
    int length = str_len(str) - 1;
    char* new_str = (char*) calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; ++i) {
        new_str[i] = str[length - i - 1];
    }
    new_str[length] = '\0';
    return new_str;
}

char* flag_u(char *str){
    int length = str_len(str);
    char* new_str = (char*) calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; ++i) {
        if (i % 2 == 1){
            new_str[i] = (char)toupper(str[i]);
        }
        else{
            new_str[i] = str[i];
        }
    }
    new_str[length] = '\0';
    return new_str;
}

char* flag_n(char *str){
    int length = str_len(str);
    char* new_str = (char*) calloc(length + 1, sizeof(char));
    int n = 0;
    for (int i = 0; i < length; ++i) {
        if(isdigit(str[i])){
            new_str[n] = str[i];
            n++;
        }
    }
    for (int i = 0; i < length; ++i) {
        if(isalpha(str[i])){
            new_str[n] = str[i];
            n++;
        }
    }
    for (int i = 0; i < length; ++i) {
        if(!isdigit(str[i]) && !isalpha(str[i])){
            new_str[n] = str[i];
            n++;
        }
    }
    new_str[n] = '\0';
    return new_str;
}

char* flag_c(int argc, char** argv){
    int seed = atoi(argv[3]);
    srand(seed);
    int res_length = str_len(argv[2]);
    char* new_str = (char*) calloc(res_length + 1, sizeof(char));
    for (int i = 0; i < res_length; ++i) {

        new_str[i] = argv[2][i];
    }
    int range = argc - 4;
    for (int i = 0; i < range; i++) {
        int ind1 = rand() % range + 4;
        int ind2 = rand() % range + 4;
        char* temp = argv[ind1];
        argv[ind1] = argv[ind2];
        argv[ind2] = temp;
    }
    for (int i = 4; i < argc; ++i) {
        int temp_length = str_len(argv[i]) - 1;
        int n = res_length - 1;
        res_length += temp_length;
        realloc(new_str, res_length + 1);
        for (int j = 0; j < temp_length; ++j) {
            new_str[n + j] = argv[i][j];
        }
    }
    new_str[res_length - 1] = '\0';
    return new_str;
}

CODE_RESULT flag_caller(int argc, char** argv){
    char flag = argv[1][1];
    printf("Given string: %s\nNew string  : ", argv[2]);
    switch (flag) {
        case 'l':
            printf("%d\n", flag_l(argv[2])); break;
        case 'r':
            printf("%s\n", flag_r(argv[2])); free(flag_r(argv[2])); break;
        case 'u':
            printf("%s\n", flag_u(argv[2])); free(flag_u(argv[2])); break;
        case 'n':
            printf("%s\n", flag_n(argv[2])); free(flag_n(argv[2])); break;
        case 'c':
            printf("%s\n", flag_c(argc, argv)); free(flag_c(argc, argv)); break;
        default:
            return WRONG_FLAG;
    }
    return OK;
}

int main(int argc, char **argv) {
    CODE_RESULT st = format_validation(argc, argv);
    if (st == OK) {
        status(flag_caller(argc, argv));
    } else {
        status(st);
    }
    return 0;
}
