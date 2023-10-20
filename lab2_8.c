#include <string.h>
#include <ctype.h>
#include "stdio.h"
#include "stdarg.h"
#include "malloc.h"

typedef enum{
    OK = 1,
    MEMORY_ERROR,
}CODE_RESULT;

void status(CODE_RESULT flag){
    switch (flag) {
        case OK:
            printf("Done\n"); break;
        case MEMORY_ERROR:
            printf("Memory error!\n"); break;
    }
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
    str = add_zeros(str, length - (int)strlen(str));
    char* res = (char*)calloc(length + 1, sizeof(char));
    if (res == NULL){
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

//TODO action
//char --> CODE_RESULT
//string validation
//memory clearing for str_sum; number0_skip, str_sum

char* action(int n, int cnt, ...){
    char* res = "0";
    va_list str;
    va_start(str, cnt);
    for (int i = 0; i < cnt; ++i) {
        res = str_sum(res, va_arg(str, char*), n);
    }
    return res;
}


int main(){
    printf("%s", action( 16, 2, "ABCDE", "A"));
    return 0;
}