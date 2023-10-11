#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "string.h"

typedef enum {
    OK = 1,
    WRONG_FORMAT,
    NAN_GIVEN,
    WRONG_FLAG,
    NO_FLAG,
    TOO_BIG,
    COMPOSITE,
    PRIME,
} CODE_RESULT;

void status(CODE_RESULT flag){
    switch (flag) {
        case WRONG_FLAG:
            printf("Flag is not valid! Possible flags: -h, -p, -s, -e, -a, -f.\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\nInput format:\n./program [number] [-key]\n"); break;
        case NAN_GIVEN:
            printf("Not a number given!\n"); break;
        case NO_FLAG:
            printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n"); break;
        case TOO_BIG:
            printf("Entered number is bigger than 10!\n"); break;
        case OK:
            printf("Done!\n"); break;
        default:
            break;
    }
}

int int_number_checker(char *s){
    if (strlen(s) > 1){
        for (int i = 0; i < strlen(s); ++i) {
            if (s[i] < '0' || s[i] > '9'){
                return 0;
            }
        }
        return 1;
    }
    else{
        return !(s[0] < '0' || s[0] > '9');
    }
}

CODE_RESULT format_validation(int argc, char **argv){
    if (argc != 3){
        return WRONG_FORMAT;
    }
    if (!int_number_checker(argv[1])){
        return NAN_GIVEN;
    }
    char prefix = argv[2][0];
    char letter = argv[2][1];
    if (prefix == '/' || prefix == '-'){
        if (letter == 'h' || letter == 'p' || letter == 's' || letter == 'e' || letter == 'a' || letter == 'f'){
            return OK;
        }
        return WRONG_FLAG;
    }
    return NO_FLAG;
}

CODE_RESULT flag_h(int number){
    for (int i = 1; i <= 100; ++i) {
        if (number % i == 0) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return OK;
}

CODE_RESULT flag_p(int number) {
    for (int i = 2; i < sqrt((double) number); ++i) {
        if (number % i == 0) {
            printf("The number %d is composite.\n", number);
            return COMPOSITE;
        }
    }
    printf("The number %d is prime.\n", number);
    return PRIME;
}

CODE_RESULT flag_s(char *argv) {
    unsigned long long temp = strlen(argv);
    for (int i = 0; i < temp; ++i) {
        printf("%c ", argv[i]);
    }
    printf("\n");
    return OK;
}

CODE_RESULT flag_e(int number) {
    if (number > 10) {
        return TOO_BIG;
    } else {
        for (int i = 1; i <= 10; ++i) {
            for (int j = 1; j <= number; ++j) {
                printf("%.0f ^ %.0f = %.0f ", (double) i, (double) j, pow(i, j));
            }
            printf("\n");
        }
        return OK;
    }
}

CODE_RESULT flag_a(int number) {
    int temp = 0;
    for (int i = 1; i <= number; ++i) {
        temp += i;
    }
    printf("%d\n", temp);
    return OK;
}

CODE_RESULT flag_f(int number) {
    int temp = 1;
    for (int i = 2; i <= number; ++i) {
        temp *= i;
    }
    printf("%d\n", temp);
    return OK;
}

void flag_caller(char** argv){
    int number = strtol(argv[1], NULL, 10);
    char *number_str = argv[1];
    char flag = argv[2][1];
    switch (flag) {
        case 'h':
            flag_h(number); break;
        case 'p':
            flag_p(number); break;
        case 's':
            flag_s(number_str); break;
        case 'e':
            if (flag_e(number) != OK){
                status(flag_e(number));
            }
            break;
        case 'a':
            flag_a(number); break;
        case 'f':
            flag_f(number); break;
        default:
            printf("Flag is not valid! Possible flags: -h, -p, -s, -e, -a, -f.\n"); break;
    }
}

void greetings(){
    printf("------------------------Input format: ./program [number] [-flag]------------------------\n");
    printf("------------------------Available flags: -h, -p, -s, -e, -a, -f-------------------------\n");
    printf("-------------------------You have to use '/' or '-' before flag-------------------------\n");
    printf("--------------------------------------Flag info-----------------------------------------\n");
    printf("----'h' printing out all natural numbers under 100 that are multiples a given number----\n");
    printf("-------------'-p' checks out if the given number is prime or composite------------------\n");
    printf("------------------------'-s' printing out digits of given number------------------------\n");
    printf("--------'-e' printing out table of degrees from 1 to with bases from 1 to number--------\n");
    printf("----------------'-a' calculate sum of all numbers from 1 to given number----------------\n");
    printf("-------------------------'-f' calculates factorial of given number----------------------\n");
    printf("\n\n");
}


int main(int argc, char **argv) {
    greetings();
    if (format_validation(argc, argv) == OK) {
        flag_caller(argv);
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}
