#include "stdio.h"
#include "stdarg.h"
#include "math.h"

#define epsilon 0.0000001

typedef enum{
    OK,
    DIVIDING_ZERO,
    WRONG_FORMAT,
}CODE_RESULT;

void status(CODE_RESULT flag){
    switch (flag){
        case DIVIDING_ZERO:
            printf("0 can not be in negative power!\n"); break;
        case OK:
            printf("Done!\n"); break;
        case WRONG_FORMAT:
            printf("Amount of numbers can not be under 1!\n"); break;
    }
}

double geom(double *res, int cnt, ...){
    if (cnt <= 0){
        return WRONG_FORMAT;
    }
    double temp_res = 1;
    int temp = cnt;
    va_list ptr;
    va_start(ptr, cnt);
    for (; cnt > 0; cnt--) {
        temp_res *= va_arg(ptr, double);
    }
    va_end(ptr);
    temp_res = pow(temp_res, 1.0/temp);
    *res = temp_res;
    return OK;
}

CODE_RESULT Pow(int n, double x, double *res){
    if (n == 0) {
        *res = 1.0;
        return OK;
    } else if (n == 1) {
        *res = x;
        return OK;
    } else if (n > 1) {
        if (fabs(x) < epsilon){
            *res = 0.0;
            return OK;
        }
        if (n % 2 == 1) {
            return Pow((n - 1) / 2, x * x, res) * x;
        } else {
            return Pow(n / 2, x * x, res);
        }
    }
    else{
        if (fabs(x) < epsilon){
            return DIVIDING_ZERO;
        }
        if (n % 2 == 0){
            return 1.0 / Pow(n / 2, x * x, res);
        }
        else{
            return 1.0 / x / Pow((n + 1) / 2, x * x, res);
        }
    }
}


int main(){
    double Pow_res, Geom_res;
    CODE_RESULT st = Pow(-5, 0.0, &Pow_res);
    if (st == OK)
        printf("%f\n", Pow_res);
    else
        status(st);
    printf("\n");
    st = geom(&Geom_res, 3, 4.5, 164.0, 7890.0);
    if (st == OK)
        printf("%f\n", Geom_res);
    else
        status(st);
    return 0;
}