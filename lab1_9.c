#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "stdio.h"
#include "time.h"

#define MAX_LENGTH 300
#define FIRST_ARRAY_SIZE 10

typedef enum{
    OK = 1,
    WRONG_FORMAT,
    WRONG_RANGE,
    NON_INT,
    MEMORY_ERROR,
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
        return WRONG_FORMAT;
    }
    if (int_checker(argv[1]) == OK && int_checker(argv[2]) == OK){
        int a = strtol(argv[1], NULL, 10);
        int b = strtol(argv[2], NULL, 10);
        if (b <= a){
            return WRONG_RANGE;
        }
        return OK;
    }
    return NON_INT;
}

void max_min_swap(int *array, int a, int b, int array_size){
    int max = a, max_i, min = b, min_i;
    for (int i = 0; i < array_size; ++i) {
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

CODE_RESULT second_task(){
    const int array_size = rand() % (10000 - 10) + 10;
    int *A = malloc(sizeof(int) * array_size);
    if (A == NULL){
        return MEMORY_ERROR;
    }
    int *B = malloc(sizeof(int) * array_size);
    if (B == NULL){
        free(A);
        return MEMORY_ERROR;
    }
    int *C = malloc(sizeof(int) * array_size);
    if (C == NULL){
        free(A);
        free(B);
        return MEMORY_ERROR;
    }
    for (int i = 0; i < array_size; ++i) {
        A[i] = rand() % 2000 - 1000;
        B[i] = rand() % 2000 - 1000;
    }
    for (int i = 0; i < array_size; ++i) {
        C[i] = A[i] + number_finder(A[i], B, array_size);
        printf("%d ",C[i]);
    }
    free(A);
    free(B);
    free(C);
    return OK;
}

void status(CODE_RESULT flag){
    switch (flag) {
        case NON_INT:
            printf("Given numbers must be integer!\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format! Input format: ./[program] [a] [b]\n"); break;
        case WRONG_RANGE:
            printf("b must be bigger than a!\n"); break;
        case OK:
            printf("Done!\n"); break;
        case MEMORY_ERROR:
            printf("Memory error!\n"); break;
    }
}

int main(int argc, char** argv){
    srand(time(NULL));
    if (format_validation(argc, argv) == OK){
        int a = strtol(argv[1], NULL, 10);
        int b = strtol(argv[2], NULL, 10);
        int array[FIRST_ARRAY_SIZE];
        for (int i = 0; i < FIRST_ARRAY_SIZE; ++i) {
            array[i] = rand() % (b - a + 1) + a;
        }
        max_min_swap(array, a, b, FIRST_ARRAY_SIZE);
    }
    else
        status(format_validation(argc, argv));
    int flag = 0;
    char input[MAX_LENGTH];
    printf("Do you want to run 2nd part of task? Enter 'Yes' to run or anything other to quit\n");
    scanf("%s", input);
    if (strcmp(input, "Yes") == 0) {
        flag = 1;
    }
    if(flag == 1){
        printf("Making 2nd part of task\n");
        status(second_task());
    }
    return 0;
}