#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stdio.h"


typedef enum{
    OK = 1,
    WRONG_FORMAT,
    WRONG_FLAG,
    NO_FLAG,
    NO_ROOTS,
    MISSING_ROOTS,
    NO_MULTIPLE,
    MULTIPLE,
    SQUARE,
    NO_SQUARE,
    NON_EXIST,
    NAN_GIVEN,
    NEGATIVE_VALUE,
} CODE_RESULT;

int str_to_i(char* number){
    int result = 0;
    unsigned long long digits = strlen(number);
    for (int i = 0; i < digits; ++i) {
        result = result * 10 + (number[i] - '0');
    }
    return result;
}

CODE_RESULT int_number_checker(char *s){
    if (strlen(s) > 1){
        for (int i = 0; i < strlen(s); ++i) {
            if (s[i] < '0' || s[i] > '9'){
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

CODE_RESULT Pythagoras(double hypo, double b, double c){
    if (pow(hypo, 2) == (pow(b, 2) + pow(c, 2))){
        printf("Triangle with sides:\n%lf\n%lf\n%lf\nis square\n", hypo, b, c);
        return SQUARE;
    }
    printf("Triangle with sides:\n%lf\n%lf\n%lf\nis not square\n", hypo, b, c);
    return NO_SQUARE;
}

CODE_RESULT finding_sides(double a, double b, double c, double epsilon){
    if ((a - b) > epsilon && (a - c) > epsilon){
        if ((a - b - c) >= epsilon){
            printf("Triangle with this sides doesn't exist\n");
            return NON_EXIST;
        }
        return Pythagoras(a, b, c);
    }
    if ((b - a) > epsilon && (b - c) > epsilon){
        if ((b - a - c) >= epsilon){
            printf("Triangle with this sides doesn't exist\n");
            return NON_EXIST;
        }
        return Pythagoras(b, a, c);
    }
    if ((c - a) > epsilon && (c - b) > epsilon){
        if ((c - a - b) >= epsilon){
            printf("Triangle with this sides doesn't exist\n");
            return NON_EXIST;
        }
        return Pythagoras(c, a, b);
    }
    return NO_SQUARE;
}

CODE_RESULT format_validation(int argc, char **argv){
    char prefix = argv[1][0];
    char letter = argv[1][1];
    if (prefix == '/' || prefix == '-'){
        if (letter == 'q'){
            if (argc != 6){
                printf("Invalid format!\nFor flag -q input format is:\n./[program] [key] [epsilon] [number1] [number2] [number3]\n");
                return WRONG_FORMAT;
            }
            if (double_number_checker(argv[2]) == OK && double_number_checker(argv[3]) == OK && double_number_checker(argv[4]) == OK &&
                double_number_checker(argv[5]) == OK){
                if (argv[2][0] == '-'){
                    printf("Negative epsilon given!");
                    return NEGATIVE_VALUE;
                }
                return OK;
            }
            printf("Not a number given!\n");
            return NAN_GIVEN;
        }
        else if (letter == 'm'){
            if (argc != 4){
                printf("Invalid format!\nFor flag -m input format is:\n./[program] [key] [number1] [number1]\n");
                return WRONG_FORMAT;
            }
            if (int_number_checker(argv[2]) == OK && int_number_checker(argv[3]) == OK) {
                return OK;
            }
            printf("Not a integer number given! Please, input integer number.\n");
            return NAN_GIVEN;
        }
        else if (letter == 't'){
            if (argc != 6){
                printf("Invalid format!\nFor flag -t input format is:\n./[program] [key] [epsilon] [number1] [number2] [number3]\n");
                return WRONG_FORMAT;
            }
            if (double_number_checker(argv[2]) == OK && double_number_checker(argv[3]) == OK && double_number_checker(argv[4]) == OK &&
                    double_number_checker(argv[5]) == OK){
                if (argv[2][0] == '-'){
                    printf("Negative epsilon given!");
                    return NEGATIVE_VALUE;
                }
                if (argv[3][0] == '-' || argv[4][0] == '-' || argv[5][0] == '-'){
                    printf("Negative value given! Sides length can't be lower than 0");
                    return NEGATIVE_VALUE;
                }
                return OK;
            }
            printf("Not a number given!\n");
            return NAN_GIVEN;
        }
        printf("Flag is not valid! Possible flags: -q, -m, -t.\n");
        return WRONG_FLAG;
    }
    printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
    return NO_FLAG;
}

CODE_RESULT equation(double a, double b, double c, double epsilon){
    // ax^2 + bx + c = 0
    double d = pow(b, 2) - 4 * a * c;
    if (d < epsilon){
        printf("NO ROOTS\n");
        return NO_ROOTS;
    }
    double x1 = (-b + sqrt(d)) / (2 * a);
    double x2 = (-b + sqrt(d)) / (2 * a);
    printf("X1 = %lf X2 = %lf\n", x1, x2);
    return OK;
}

CODE_RESULT combinations(double x, double y, double z, double epsilon){
    double temp;
    int counter = 0;
    for (int i = 0; i < 3; ++i) {
        printf("%lf %lf %lf : ", x, y, z);
        equation(x, y, z, epsilon);
        if (equation(x, y, z, epsilon) != OK){
            counter++;
        }
        temp = y;
        y = z;
        z = temp;
        printf("%lf %lf %lf : ", x, y, z);
        equation(x, y, z, epsilon);
        if (equation(x, y, z, epsilon) != OK){
            counter++;
        }
        temp = x;
        x = y;
        y = temp;
    }
    if (counter == 6){
        return NO_ROOTS;
    }
    else if (counter > 0){
        return MISSING_ROOTS;
    }
    return OK;
}

CODE_RESULT flag_q(char **argv){
    double epsilon = strtod(argv[2], NULL);
    double a = strtod(argv[3], NULL);
    double b = strtod(argv[4], NULL);
    double c = strtod(argv[5], NULL);
    combinations(a, b , c, epsilon);
    return OK;
}

CODE_RESULT flag_m(char **argv){
    int num1 = str_to_i(argv[2]);
    int num2 = str_to_i(argv[3]);
    if (abs(num1) % abs(num2)){
        printf("%d is not multiple of %d\n", num1, num2);
        return NO_MULTIPLE;
    }
    printf("%d is multiple of %d\n", num1, num2);
    return MULTIPLE;
}

CODE_RESULT flag_t(char **argv){
    double epsilon = strtod(argv[2], NULL);
    double a = strtod(argv[3], NULL);
    double b = strtod(argv[4], NULL);
    double c = strtod(argv[5], NULL);
    finding_sides(a, b, c, epsilon);
    return OK;
}

void flag_caller(char **argv){
    char flag = argv[1][1];
    switch (flag) {
        case 'q':
            flag_q(argv); break;
        case 'm':
            flag_m(argv); break;
        case 't':
            flag_t(argv); break;
        default:
            printf("Flag is not valid! Possible flags: -q, -m, -t.\n"); break;
    }
}

void greetings(){
    printf("\n\n");
    printf("-----------------------------Available flags: -q, -m, -t--------------------------------\n");
    printf("-------------------------You have to use '/' or '-' before flag-------------------------\n");
    printf("--------------------------------------Flag info-----------------------------------------\n");
    printf("---------'q' printing out all possible solutions for equation ax^2 + bx + c = 0---------\n");
    printf("-----------with all possible permutations of a, b, c for given numbers------------------\n");
    printf("----Input format for 'q': ./[program] [key] [epsilon] [number1] [number2] [number3]-----\n");
    printf("---------------------'m' checks out if number1 multiples of number2---------------------\n");
    printf("---------------Input format for 'm': ./[program] [key] [number1] [number1]--------------\n");
    printf("-------------'t' checks out if given numbers can be sides of square triangle------------\n");
    printf("----Input format for 't': ./[program] [key] [epsilon] [number1] [number2] [number3]-----\n");
    printf("\n\n");
}

int main(int argc, char **argv){
    greetings();
    if (format_validation(argc, argv) == OK){
        flag_caller(argv);
    }
}