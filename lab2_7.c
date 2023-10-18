#include <math.h>
#include "stdio.h"

double func_1(double x){
    return exp(x) - 2.0;
}

double func_2(double x){
    return log(x) + x - 4;
}

double func_3(double x){
    return sqrt(x) + x - 10;
}

double equation(double l_border, double r_border, double epsilon, double (*function)(double)){
    double central;
    while (fabs(l_border - r_border) > epsilon){
        central = (l_border + r_border) / 2;
        if (fabs(function(central)) < epsilon){
            return central;
        }
        if (function(central) > epsilon){
            r_border = central;
        }
        else {
            l_border = central;
        }
    }
    return (l_border + r_border) / 2;
}

int main(){
    printf("%f\n", equation(0.0, 1.0, 0.00001, func_1));
    printf("%f\n", equation(-10.0, 54.8, 0.00001, func_2));
    printf("%f\n", equation(-100.0, 100.0, 0.00001, func_3));
    return 0;
}