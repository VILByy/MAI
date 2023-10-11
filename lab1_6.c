#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#include "math.h"

typedef enum{
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

double a_func(double x){
    return log(1 + x) / x;
}

double b_func(double x){
    return pow(M_E, -x*x/2);
}

double c_func(double x){
    return log(1/(1 - x));
}

double d_func(double x){
    return pow(x, x);
}

double integral(double (*func)(double), double epsilon, int side){
    // side == 1 -> going from left limit
    // side == 0 -> going from right limit
    double a = 0.0, b = 1.0;
    double res = 0;
    double acc = 2.0;
    double prev;
    do{
        double step = (b - a) / acc;
        prev = res;
        res = 0;
        if (side == 1) {
            for (double cur = b; cur > a + epsilon; cur -= step) {
                res += step * func(cur);
            }
        }
        else{
            for (double cur = a; cur < b - epsilon; cur += step) {
                res += step * func(cur);
            }
        }
        acc++;
    }while (fabs(prev - res) > epsilon);
    return res;
}


CODE_RESULT format_validation(int argc, char **argv){
    if (argc != 2){
        printf("Wrong format!\n");
        return WRONG_FORMAT;
    }
    if(double_number_checker(argv[1]) == NAN_GIVEN){
        printf("Not a number given!\n");
        return NAN_GIVEN;
    }
    return OK;
}

void printer(double epsilon){
    printf("a. %.20f\n", integral(a_func, epsilon, 1));
    printf("b. %.20f\n", integral(b_func, epsilon, 1));
    printf("c. %.20f\n", integral(c_func, epsilon, 0));
    printf("d. %.20f\n", integral(d_func, epsilon, 1));
}

void greetings(){
    printf("-------------Input format-----------\n");
    printf("--------./[program] [epsilon]-------\n");
    printf("---Program calculates 4 integrals---\n");
    printf("--------with epsilon accuracy-------\n");
    printf("\n");
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

int main(int argc, char **argv){
    greetings();
    if(format_validation(argc, argv) == OK){
        printer(strtod(argv[1], NULL));
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}