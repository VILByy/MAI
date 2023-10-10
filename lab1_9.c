#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "stdio.h"
#include "time.h"

typedef enum{
    OK = 1,
    WRONG_FORMAT,
}CODE_RESULT;

int number_finder(int number, int *array, int array_size){
    int res = 1001;
    for (int i = 0; i < array_size; ++i) {
        if (abs(array[i] - number) < abs(res - number)){
            res = array[i];
        }
    }
    return res;
}

CODE_RESULT int_checker(char* number){
    int length = (int)strlen(number);
    for (int i = 0; i < length; ++i) {
        if (!isdigit(number[i])){
            return WRONG_FORMAT;
        }
    }
    return OK;
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc != 3){
        printf("Wrong format! Input format: ./[program] [a] [b]\n");
        return WRONG_FORMAT;
    }
    if (int_checker(argv[1]) == OK && int_checker(argv[2]) == OK){
        int a = strtol(argv[1], NULL, 10);
        int b = strtol(argv[2], NULL, 10);
        if (b <= a){
            printf("b must be bigger than a!\n");
            return WRONG_FORMAT;
        }
        return OK;
    }
    printf("Given numbers must be integer!\n");
    return WRONG_FORMAT;
}

int main(int argc, char** argv){
    srand(time(NULL));
    if (format_validation(argc, argv) == OK){
        int a = strtol(argv[1], NULL, 10);
        int b = strtol(argv[2], NULL, 10);
        const int array_size = 10;
        int array[array_size];
        int max = a, max_i, min = b, min_i;
        for (int i = 0; i < array_size; ++i) {
            array[i] = rand() % (b - a + 1) + a;
            if (array[i] > max){
                max = array[i];
                max_i = i;
            }
            if (array[i] < min){
                min = array[i];
                min_i = i;
            }
        }
        array[min_i] = max;
        array[max_i] = min;
    }
    int flag = 0;
    while(1){
        char input[10];
        printf("Do you want to run 2nd part of task? Enter 'Yes' to run or anything other to end\n");
        scanf("%s", input);
        if (strcmp(input, "Yes") == 0){
            flag = 1;
            break;
        }
        break;
    }
    if(flag == 1){
        printf("Making 2nd part of task\n");
        const int array_size = rand() % (10000 - 10) + 10;
        int *A = malloc(sizeof(int) * array_size);
        int *B = malloc(sizeof(int) * array_size);
        int *C = malloc(sizeof(int) * array_size);
        for (int i = 0; i < array_size; ++i) {
            A[i] = rand() % 2000 - 1000;
            B[i] = rand() % 2000 - 1000;
        }
        for (int i = 0; i < array_size; ++i) {
            C[i] = A[i] + number_finder(A[i], B, array_size);
        }
        free(A);
        free(B);
        free(C);
    }
    return 0;
}