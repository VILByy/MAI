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

void status(CODE_RESULT flag){
    switch (flag) {
        case WRONG_FLAG:
            printf("Flag is not valid! Possible flags: -q, -m, -t.\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case NO_FLAG:
            printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n"); break;
        case NO_ROOTS:
            printf("NO ROOTS\n"); break;
        case NAN_GIVEN:
            printf("Not a number given!\n"); break;
        case NEGATIVE_VALUE:
            printf("Negative value given!"); break;
        case OK:
            printf("Done!\n"); break;
        default:
            break;
    }
}

int comp (const void * elem1, const void * elem2){
    if (*(double*)elem1 < *(double*)elem2)
        return 1;
    else if (*(double*)elem1 > *(double*)elem2)
        return -1;
    else
        return 0;
}


int str_to_i(char* number){
    int result = 0;
    for (int i = 0; i < strlen(number); ++i) {
        result = result * 10 + (number[i] - '0');
    }
    return result;
}

CODE_RESULT int_number_checker(char *s){
    int length = (int)strlen(s);
    if (length > 1){
        for (int i = 0; i < length; ++i) {
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

CODE_RESULT Pythagoras(double hypo, double b, double c, double epsilon){
    if (fabs(pow(hypo, 2) - (pow(b, 2) + pow(c, 2))) < epsilon){
        return SQUARE;
    }
    return NO_SQUARE;
}

CODE_RESULT finding_sides(double a, double b, double c, double epsilon){
    if ((a - b - c) >= epsilon){
        return NON_EXIST;
    }
    return OK;
}

CODE_RESULT format_validation(int argc, char **argv){
    if (argc != 4 && argc != 6){
        return WRONG_FORMAT;
    }
    char prefix = argv[1][0];
    char letter = argv[1][1];
    if (prefix == '/' || prefix == '-'){
        if (letter == 'q'){
            if (argc != 6){
                return WRONG_FORMAT;
            }
            if (double_number_checker(argv[2]) == OK && double_number_checker(argv[3]) == OK && double_number_checker(argv[4]) == OK &&
                double_number_checker(argv[5]) == OK){
                if (argv[2][0] == '-'){
                    return NEGATIVE_VALUE;
                }
                return OK;
            }
            return NAN_GIVEN;
        }
        else if (letter == 'm'){
            if (argc != 4){
                return WRONG_FORMAT;
            }
            if (int_number_checker(argv[2]) == OK && int_number_checker(argv[3]) == OK) {
                return OK;
            }
            return NAN_GIVEN;
        }
        else if (letter == 't'){
            if (argc != 6){
                return WRONG_FORMAT;
            }
            if (double_number_checker(argv[2]) == OK && double_number_checker(argv[3]) == OK && double_number_checker(argv[4]) == OK &&
                    double_number_checker(argv[5]) == OK){
                if (argv[2][0] == '-'){
                    return NEGATIVE_VALUE;
                }
                if (argv[3][0] == '-' || argv[4][0] == '-' || argv[5][0] == '-'){
                    return NEGATIVE_VALUE;
                }
                return OK;
            }
            return NAN_GIVEN;
        }
        return WRONG_FLAG;
    }
    return NO_FLAG;
}

CODE_RESULT equation(double a, double b, double c, double epsilon, double *x1, double *x2){
    // ax^2 + bx + c = 0
    double d = pow(b, 2) - 4 * a * c;
    if (d < epsilon){
        return NO_ROOTS;
    }
    double sqr_d = sqrt(d);
    *x1 = (-b + sqr_d) / (2 * a);
    *x2 = (-b - sqr_d) / (2 * a);
    return OK;
}

CODE_RESULT combinations(double x, double y, double z, double epsilon){
    double temp;
    int counter = 0;
    double x1, x2;
    for (int i = 0; i < 3; ++i) {
        printf("%lf %lf %lf : ", x, y, z);
        if (equation(x, y, z, epsilon, &x1, &x2) != OK){
            status(equation(x, y, z, epsilon, &x1, &x2));
            counter++;
        }
        else{
            printf("X1 = %lf X2 = %lf\n", x1, x2);
        }
        temp = y;
        y = z;
        z = temp;
        printf("%lf %lf %lf : ", x, y, z);
        if (equation(x, y, z, epsilon, &x1, &x2) != OK){
            status(equation(x, y, z, epsilon, &x1, &x2));
            counter++;
        }
        else{
            printf("X1 = %lf X2 = %lf\n", x1, x2);
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
    double arr[3] = {a, b ,c};
    qsort(arr, 3, sizeof(double), comp);
    if(finding_sides(arr[0], arr[1], arr[2], epsilon) == OK){
        if (Pythagoras(arr[0], arr[1], arr[2], epsilon) == SQUARE){
            printf("Triangle with sides:\n%lf\n%lf\n%lf\nis square\n", arr[0], arr[1], arr[2]);
            return SQUARE;
        }
        else{
            printf("Triangle with sides:\n%lf\n%lf\n%lf\nis not square\n", arr[0], arr[1], arr[2]);
            return NO_SQUARE;
        }
    }
    printf("Triangle with sides:\n%lf\n%lf\n%lf\ndoesn't exist\n", arr[0], arr[1], arr[2]);
    return NON_EXIST;
}

CODE_RESULT flag_caller(char **argv){
    char flag = argv[1][1];
    switch (flag) {
        case 'q':
            flag_q(argv); break;
        case 'm':
            flag_m(argv); break;
        case 't':
            flag_t(argv); break;
        default:
            return WRONG_FLAG;
    }
    return OK;
}


void greetings(){
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
        status(flag_caller(argv));
    }
    else
        status(format_validation(argc, argv));
    return 0;
}