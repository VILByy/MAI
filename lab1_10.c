#include <string.h>
#include "stdio.h"
#include "malloc.h"
#include "ctype.h"

#define MAX_LENGTH 300

char letter(long long int number, int base){
    if (number % base >= 10){
        return (char)('A' + (int)(number % base - 10));
    }
    return (char)('0' + (int)(number % base));
}


char* converter(long long int number, int base){
    long long int temp = number;
    int length = 0;
    while(temp > 0){
        temp /= base;
        length++;
    }
    char* res = (char*)calloc(length + 1, sizeof(char));
    if (res == NULL) {
        printf("Memory error!");
        return NULL;
    }
    for (int i = 0; i < length; ++i) {
        res[length - 1 - i] = letter(number, base);
        number /= base;
    }
    return res;
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

void greetings(){
    printf("--------------This program reads numbers from console and finds max element-------------\n");
    printf("------------------First of all, you need to enter base of numbers [2-36]----------------\n");
    printf("---------------------------After that, enter numbers in this base-----------------------\n");
    printf("---------------------------When you end, enter 'Stop' in console------------------------\n");
    printf("-------------Program will print out absolute max in 10, 9, 18, 27, 36 base--------------\n");
    printf("\n\n");
}

int main(){
    greetings();
    char input[MAX_LENGTH];
    int base, flag, is_ok = 0;
    while(is_ok == 0) {
        flag = 0;
        scanf("%s", input);
        for (int i = 0; i < strlen(input); ++i) {
            if (!isdigit(input[i])) {
                flag = 1;
                printf("Nan given!\n");
                break;
            }
        }
        if (flag == 0) {
            base = strtol(input, NULL, 10);
            if (base < 2 || base > 36) {
                printf("Numeral can be in range 2 - 36\nPlease input allowed number: ");
                flag = 1;
            }
            if (flag == 0){
                is_ok = 1;
            }
        }
    }
    long long max = 0;
    char max_number[MAX_LENGTH];
    is_ok = 0;
    while(is_ok == 0){
        flag = 0;
        scanf("%s", input);
        if (strcmp(input, "Stop") == 0){
            is_ok = 1;
        }
        for (int i = 0; i < strlen(input); ++i) {
            if(!isdigit(input[i]) && (input[i] < 'A' || input[i] > 'Z') && (input[i] != '-')){
                flag = 1;
                printf("Wrong number format! All letters should be upper-case!\n");
                break;
            }
        }
        if (range_checker(input, base) == 0 && flag == 0){
            flag = 1;
            printf("Given number can't be in given base\n");
        }
        if (flag == 0){
            long long number = strtoll(input, NULL, base);
            if (llabs(number) > max){
                max = llabs(number);
                strcpy(max_number, input);
            }
        }
    }
    printf("%s\n10: %lld\n", max_number, max);
    for (int i = 1; i < 5; ++i) {
        printf("%d: %s\n", i * 9, converter(max, i * 9));
        free(converter(max, i * 9));
    }
    return 0;
}