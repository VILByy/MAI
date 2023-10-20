#include <string.h>
#include <ctype.h>
#include "stdio.h"
#include "stdarg.h"
#include "malloc.h"

typedef enum{
    OK = 1,
    MEMORY_ERROR,
    WRONG_NUMBER,
    WRONG_BASE,
}CODE_RESULT;

void status(CODE_RESULT flag){
    switch (flag) {
        case OK:
            printf("Done\n"); break;
        case MEMORY_ERROR:
            printf("Memory error!\n"); break;
        case WRONG_NUMBER:
            printf("Given number can not be written in given base!\n"); break;
        case WRONG_BASE:
            printf("Wrong base given! Available base are in range 2-36.\n"); break;
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

char letter(long long int number, int base){
    if (number % base >= 10){
        return (char)('A' + (int)(number % base - 10));
    }
    return (char)('0' + (int)(number % base));
}

int str_max(char* str1, char* str2){
    return (int) strlen(strlen(str1) > strlen(str2) ? str1 : str2);
}

char* add_zeros(char* str, int n){
    int length = (int)strlen(str) + n;
    char* res = (char*) calloc(length, sizeof(char));
    if (res == NULL){
        return NULL;
    }
    for (int i = 0; i < length; ++i) {
        if (i < n){
            res[i] = '0';
        }
        else{
            res[i] = str[i - n];
        }
    }
    return res;
}

char* number_zero_skipper(char *number){
    int length = (int)strlen(number);
    char* res = calloc(length + 1, sizeof(char));
    if (res == NULL){
        return NULL;
    }
    int index = 0;
    for (int i = 0; i < length; ++i) {
        if (number[i] == '0'){
            realloc(res, length - i);
        }
        if (number[i] != '0'){
            index = i;
            break;
        }
    }
    for (int i = 0; i < length - index; ++i) {
        res[i] = number[index + i];
    }
    return res;
}

int number_get_int(char c){
    c = (char)toupper(c);
    if (c >= 'A' && c <= 'Z'){
        return c - 'A' + 10;
    }
    return c - '0';
}

char* str_sum(char* prev_res, char* str, int base){
    int length = str_max(prev_res, str) + 1;
    prev_res = add_zeros(prev_res, length - (int)strlen(prev_res));
    if (prev_res == NULL){
        return NULL;
    }
    str = add_zeros(str, length - (int)strlen(str));
    if (str == NULL){
        free(prev_res);
        return NULL;
    }
    char* res = (char*)calloc(length + 1, sizeof(char));
    if (res == NULL){
        free(str);
        free(prev_res);
        return NULL;
    }
    int next = 0;
    for (int i = length - 1; i >= 0 ; --i) {
        int n1 = number_get_int(prev_res[i]);
        int n2 = number_get_int(str[i]);
        res[i] = letter((n1 + n2 + next) % base, base);
        next = (n1 + n2 + next) / base;
    }
    free(prev_res);
    free(str);
    res[length] = '\0';
    res = number_zero_skipper(res);
    return res;
}

CODE_RESULT action(char** res, int n, int cnt, ...){
    if (n < 2 || n > 36){
        return WRONG_BASE;
    }
    va_list str;
    va_start(str, cnt);
    for (int i = 0; i < cnt; ++i) {
        char* temp = va_arg(str, char*);
        if (range_checker(temp, n) == 0){
            return WRONG_NUMBER;
        }
        *res = str_sum(*res, temp, n);
        if (res == NULL){
            return MEMORY_ERROR;
        }
    }
    return OK;
}

int main(){
    char* res = "0";
    status(action(&res, 36, 3, "ABCDE", "A", "ZXCZCXZCXZCX"));
    printf("%s\n", res);
    return 0;
}