#include <math.h>
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

typedef enum {
    OK = 1,
    NAN_GIVEN,
    WRONG_FORMAT,
}CODE_RESULT;

CODE_RESULT double_number_checker(char *s){
    if (strlen(s) > 1){
        for (int i = 0; i < strlen(s); ++i) {
            if ((s[i] < '0' || s[i] > '9') && s[i] != '.'){
                return NAN_GIVEN;
            }
        }
        return OK;
    }
    else{
        if (s[0] < '0' || s[0] > '9'){
            return NAN_GIVEN;
        }
        return OK;
    }
}

double a_sum(double epsilon, double x){
    double res = 0;
    double prev;
    double cur = 1;
    int n = 0;
    do {
        prev = cur;
        res += cur;
        cur *= x/(n + 1);
        n++;
    } while (fabs(cur - prev) >= epsilon);
    return res;
}

double b_sum(double epsilon, double x){
    double res = 0;
    double prev;
    double cur = 1;
    int n = 0;
    do{
        prev = cur;
        res += cur;
        cur *= (-1) * pow(x, 2)/ ((2 * n + 1) * (2 * n + 2));
        n++;
    }while (fabs(cur - prev) >= epsilon);
    return res;
}

double c_sum(double epsilon, double x){
    double res = 0;
    double prev;
    double cur = 1;
    int n = 0;
    do{
        prev = cur;
        res += cur;
        cur *= (9 * x * x * pow(n + 1, 2)) / ((3 * n + 1) * (3 * n + 2));
        n++;
    }while (fabs(cur - prev) >= epsilon);
    return res;
}

double d_sum(double epsilon, double x){
    double res = 0;
    double prev;
    double cur = - (x * x) / 2;
    int n = 1;
    do{
        prev = cur;
        res += cur;
        cur *= (-1) * x * x * (2 * n + 1) / (2 * n + 2);
        n++;
    }while(fabs(cur - prev) >= epsilon);
    return res;
}

CODE_RESULT format_validation(int argc, char **argv){
    if (argc != 3){
        printf("Wrong input format!\n");
        return WRONG_FORMAT;
    }
    if (double_number_checker(argv[1]) == NAN_GIVEN || double_number_checker(argv[2]) == NAN_GIVEN){
        printf("Not a number given!\n");
        return NAN_GIVEN;
    }
    return OK;
}

void printer(double epsilon, double x){
    printf("a. %.20f\n", a_sum(epsilon, x));
    printf("b. %.20f\n", b_sum(epsilon, x));
    if ((1.0 - x) > epsilon){
        printf("c. %.20f\n", c_sum(epsilon, x));
        printf("d. %.20f\n", d_sum(epsilon, x));
    }
    else{
        printf("c. x must be less than 1\n");
        printf("d. x must be less than 1\n");
    }
}

void status(CODE_RESULT flag){
    switch (flag) {
        case NAN_GIVEN:
            printf("Not a number given!\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case OK:
            printf("Done!\n"); break;
    }
}

void greetings(){
    printf("------------Input format-----------\n");
    printf("-----./[program] [epsilon] [x]-----\n");
    printf("-Program calculates 4 series for x-\n");
    printf("-------with epsilon accuracy-------\n");
    printf("\n\n");
}


int main(int argc, char **argv){
    greetings();
    if (format_validation(argc, argv) == OK){
        printer(strtod(argv[1], NULL), strtod(argv[2], NULL));
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}