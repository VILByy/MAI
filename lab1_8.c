#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include "stdio.h"

typedef enum{
    OK = 1,
    WRONG_FORMAT,
    NON_TXT,
    INPUT_FILE_ERROR,
    OUTPUT_FILE_ERROR,
    MEMORY_ERROR,
}CODE_RESULT;

char* PATH_transform(char* PATH){
    int length = (int)strlen(PATH);
    for (int i = 0; i < length; ++i) {
        if (PATH[i] == '\\'){
            PATH[i] = '/';
        }
    }
    return PATH;
}

char trash_skipper(FILE *input){
    char c;
    int flag = 0;
    while(flag == 0){
        c = (char)fgetc(input);
        if (c == EOF){
            return EOF;
        }
        c = (char)tolower(c);
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')){
            flag = 1;
        }
    }
    fseek(input, -1, SEEK_CUR);
    return c;
}

char* number_zero_skipper(char *number){
    int length = (int)strlen(number);
    char* res = calloc(length + 1, sizeof(char));
    int index = 0;
    for (int i = 0; i < length; ++i) {
        if (number[i] == '0'){
            realloc(res, length - i);
        }
        if (number[i] != '0'){
            index = i;
            break;
        }
    }
    for (int i = 0; i < length - index; ++i) {
        res[i] = number[index + i];
    }
    return res;
}

char* number_finder(FILE* input){
    char* name = (char*) calloc(2, sizeof(char));
    if (name == NULL){
        return NULL;
    }
    char c;
    int i = 0, flag = 0;
    while(flag == 0){
        c = (char)fgetc(input);
        c = (char)tolower(c);
        realloc(name, i + 1);
        if(c == EOF){
            flag = 1;
        }
        if((c < '0' || c > '9') && (c < 'a' || c > 'z')) {
            flag = 1;
        }
        name[i] = c;
        i++;
    }
    name[i - 1] = '\0';
    return name;
}

int numeral_system(char* number){
    int MAX = 0;
    int length = (int)strlen(number);
    int temp[length];
    for (int i = 0; i < length; ++i) {
        if (number[i] >= 'a' && number[i] <= 'z'){
            temp[i] = number[i] - 'a' + 10; // 10 digits
        }
        else{
            temp[i] = number[i] - '0';
        }
        if(MAX < temp[i]){
            MAX = temp[i];
        }
    }
    return MAX + 1;
}

CODE_RESULT file_extension(char *name){
    int index = (int)strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.'){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc != 3){
        return WRONG_FORMAT;
    }
    if (file_extension(argv[1]) == OK && file_extension(argv[2]) == OK){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT action(char** argv){
    FILE *input = fopen(PATH_transform(argv[1]), "r");
    FILE *output = fopen(PATH_transform(argv[2]), "w");
    if (input == NULL){
        fclose(output);
        return INPUT_FILE_ERROR;
    }
    if (output == NULL){
        fclose(input);
        return OUTPUT_FILE_ERROR;
    }
    int flag = 0;
    while (flag == 0){
        char* number = number_finder(input);
        if (number == NULL){
            fclose(input);
            fclose(output);
            return MEMORY_ERROR;
        }
        fprintf(output, "%s %d %lld\n", number_zero_skipper(number), numeral_system(number), strtoll(number, NULL, numeral_system(number)));
        free(number_zero_skipper(number));
        free(number);
        if (trash_skipper(input) == EOF)
            flag = 1;
    }
    fclose(input);
    fclose(output);
    return OK;
}

void status(CODE_RESULT flag){
    switch (flag) {
        case NON_TXT:
            printf("Non txt file given!\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case INPUT_FILE_ERROR:
            printf("Unable to open input file!\n"); break;
        case OUTPUT_FILE_ERROR:
            printf("Unable to open output file!\n"); break;
        case MEMORY_ERROR:
            printf("Memory error!\n"); break;
        case OK:
            printf("Done!\n"); break;
    }
}

void greetings(){
    printf("--------------This program reads numbers from input file and finds max base-------------\n");
    printf("--------------It will print out numbers to output file following this rules:------------\n");
    printf("----------------------------Every number printed on new string--------------------------\n");
    printf("------------------------ORIGINAL_NUMBER MAX_BASE NUMBER_IN_10_BASE----------------------\n");
    printf("\n\n");
}

int main(int argc, char** argv){
    if (format_validation(argc, argv) == OK){
        status(action(argv));
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}