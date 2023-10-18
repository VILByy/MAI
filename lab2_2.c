#include "stdio.h"
#include "stdarg.h"
#include "math.h"

#define epsilon 0.000001

double geom(int cnt, ...){
    double res = 1;
    int temp = cnt;
    va_list ptr;
    va_start(ptr, cnt);
    for (; cnt > 0; cnt--) {
        res *= va_arg(ptr, double);
    }
    va_end(ptr);
    res = pow(res, 1.0/temp);
    return res;
}

double grade(int n, double x){
    if (fabs(x) < epsilon){
        return 0.0;
    }
    if (n == 0){
        return 1.0;
    }
    if (n == 1){
        return x;
    }
    if (n % 2){
        return x * grade((n - 1) / 2, x * x);
    }
    return grade(n / 2, x * x);
}

int main(){
    printf("%f\n", geom(3, 4.5, 164.0, 7890.0));
    printf("%f\n"), grade(10, 8.0);
    return 0;
}