#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#include "math.h"

int is_prime(int number) {
    for (int i = 2; i < sqrt((double) number); ++i) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

int factorial(int i){
    if(i > 20)
        i = 20;
    int res = 1;
    for (int j = 1; j <= i; ++j) {
        res *= j;
    }
    return res;
}

double e_from_row(double epsilon){
    int n = 0;
    double e = 0, e_prev;
    do{
        e_prev = e;
        e += pow(factorial(n), -1);
        n++;
    }while(fabs(e - e_prev) >= epsilon);
    return e;
}

double e_from_lim(double epsilon){
    int n = 1;
    double e = 1, prev_e;
    do{
        prev_e = e;
        e = pow((1 + pow(n, -1)), n);
        n++;
    }while((fabs(e - prev_e) >= epsilon));
    return e;
}

double e_from_equation(double epsilon){
    double l_border = 1.0;
    double r_border = 3.0;
    double central;
    while (fabs(l_border - r_border) > epsilon){
        central = (l_border + r_border) / 2;
        double function = log(central) - 1;
        if (fabs(function) < epsilon){
            return central;
        }
        if ((log(l_border) - 1) * (log(central) - 1) < 0){
            r_border = central;
        }
        else {
            l_border = central;
        }
    }
    return (l_border + r_border) / 2;
}

double pi_from_lim(double epsilon){
    int n = 1;
    double pi = 4.0, prev_pi;
    do{
        prev_pi = pi;
        pi = pi * (4.0 * n * (n + 1.0)) / pow((2.0 * n + 1.0), 2);
        n++;
    }while(fabsl(pi - prev_pi) >= epsilon);
    return pi;
}

double pi_from_row(double epsilon){
    int n = 1;
    double pi = 0, pi_prev;
    do{
        pi_prev = pi;
        pi += pow(-1.0, n - 1) / (2 * n - 1);
        n++;
    }while(fabs(pi - pi_prev) >= epsilon);
    return 4 * pi;
}

double pi_from_equation(double epsilon){
    double l_border = 2.0;
    double r_border = 4.0;
    double central;
    while (fabs(l_border - r_border) > epsilon){
        central = (l_border + r_border) / 2;
        double function = cos(central) + 1;
        if (fabs(function) < epsilon){
            return central;
        }
        if (sin(central) > 0){
            l_border = central;
        }
        else {
            r_border = central;
        }
    }
    return (l_border + r_border) / 2;
}

double ln_from_lim(double epsilon){
    int n = 1;
    double ln = 1.0, prev_ln;
    do{
        prev_ln = ln;
        ln = ln * (n + 1.0) * (pow(2.0, pow(n + 1, -1)) - 1.0) / n / (pow(2.0, pow(n, -1)) - 1.0);
        n++;
    }while(fabsl(ln - prev_ln) >= epsilon);
    return ln;
}

double ln_from_row(double epsilon){
    int n = 1;
    double ln = 0, ln_prev;
    do{
        ln_prev = ln;
        ln += pow(-1.0, n - 1) / n;
        n++;
    }while(fabs(ln - ln_prev) >= epsilon);
    return ln;
}

double ln_from_equation(double epsilon){
    double l_border = 0.0;
    double r_border = 1.0;
    double central;
    while (fabs(l_border - r_border) > epsilon){
        central = (l_border + r_border) / 2;
        double function = exp(central) - 2;
        if (fabs(function) < epsilon){
            return central;
        }
        if (function > epsilon){
            r_border = central;
        }
        else {
            l_border = central;
        }
    }
    return (l_border + r_border) / 2;
}

double sq_from_lim(double epsilon){
    int n = 0;
    double sq = - 0.5, prev_sq;
    do{
        prev_sq = sq;
        sq = sq - (pow(sq, 2) / 2) + 1;
        n++;
    }while(fabsl(sq - prev_sq) >= epsilon);
    return sq;
}

double sq_from_row(double epsilon){
    int n = 2;
    double sq = 1, sq_prev;
    do{
        sq_prev = sq;
        sq *= pow(2, pow(2, -n));
        n++;
    }while(fabs(sq - sq_prev) >= epsilon);
    return sq;
}

double sq_from_equation(double epsilon){
    double l_border = 1.0;
    double r_border = 2.0;
    double central;
    while (fabs(l_border - r_border) > epsilon){
        central = (l_border + r_border) / 2;
        double function = pow(central, 2) - 2;
        if (fabs(function) < epsilon){
            return central;
        }
        if (function > epsilon){
            r_border = central;
        }
        else {
            l_border = central;
        }
    }
    return (l_border + r_border) / 2;
}

double sum_gamma(int n){
    double res = 1.0;
    for (int i = 2; i < n; ++i) {
        res += pow(i, -1);
    }
    return res - log(n);
}

double gamma_from_lim(double epsilon){
    double gamma = 0, prev_gamma;
    int n = 1;
    do{
        prev_gamma = gamma;
        gamma = sum_gamma(n);
        n++;
    }while(fabsl(gamma - prev_gamma) >= epsilon);
    return gamma;
}

double gamma_from_row(double epsilon){
    int k = 2;
    double gamma = 0.5;
    double gamma_prev;
    do{
        gamma_prev = gamma;
        k++;
        if(fmod(sqrt(k), 1.0) < epsilon) {
            k++;
        }
        gamma += ((1.0 / (pow(floor(sqrt(k)), 2))) - (1.0 / k));
    }while(fabs(gamma - gamma_prev) > epsilon);
    return (-1.0) * pow(M_PI, 2) / 6 + gamma;
}

double gamma_comp(double epsilon){
    int p = 2;
    double gamma = log(2) * 0.5;
    double prev_gamma;
    double composition = 0.5;
    do {
        prev_gamma = gamma;
        do {
            p++;
        } while (!is_prime(p));
        composition *= (p - 1.0) / p;
        gamma = log(p) * composition;
    } while (fabs(prev_gamma - gamma) > epsilon);
    return gamma;
}

double gamma_from_equation(double epsilon){
    double l_border = 0.0;
    double r_border = 100.0;
    double central;
    while (fabs(l_border - r_border) > epsilon){
        central = (l_border + r_border) / 2;
        double function = exp((-1.0 * central)) - gamma_comp(epsilon);
        if (fabs(function) < epsilon){
            return central;
        }
        if (function > epsilon){
            r_border = central;
        }
        else {
            l_border = central;
        }
    }
    return (l_border + r_border) / 2;
}

void printer(double epsilon){
    printf("e from lim:          %.20f\tpi from lim:      %.20f\n", e_from_lim(epsilon), pi_from_lim(epsilon));
    printf("e from row:          %.20f\tpi from row:      %.20f\n", e_from_row(epsilon), pi_from_row(epsilon));
    printf("e from equation:     %.20f\tpi from equation: %.20f\n", e_from_equation(epsilon), pi_from_equation(epsilon));
    printf("\n");
    printf("ln from lim:         %.20f\tsq from lim:      %.20f\n", ln_from_lim(epsilon), sq_from_lim(epsilon));
    printf("ln from row:         %.20f\tsq from row:      %.20f\n", ln_from_row(epsilon), sq_from_row(epsilon));
    printf("ln from equation:    %.20f\tsq from equation: %.20f\n", ln_from_equation(epsilon), sq_from_equation(epsilon));
    printf("\n");
    printf("gamma from lim:      %.20f\n", gamma_from_lim(epsilon));
    printf("gamma from row:      %.20f\n", gamma_from_row(epsilon));
    printf("gamma from equation: %.20f\n", gamma_from_equation(epsilon));
}

int main(int argc, char **argv){
    if (argc != 2){
        printf("Incorrect format!\nInput format:\n./program [epsilon]\n");
        return 0;
    }
    double epsilon = strtod(argv[1], NULL);
    printer(epsilon);
    return 0;
}
