#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <math.h>
#include "stdio.h"

typedef enum{
    OK = 1,
    WRONG_FORMAT,
    NON_TXT,
    INPUT_FILE_ERROR,
    OUTPUT_FILE_ERROR,
}CODE_RESULT;

int converter(char* number_in, int numeral){
    int length = (int) strlen(number_in);
    int number[length];
    for (int i = 0; i < length; ++i) {
        if (number_in[i] >= 'a' && number_in[i] <= 'z'){
            number[i] = number_in[i] - 'a' + 1;
        }
        else{
            number[i] = number_in[i] - '0';
        }
    }
    int res = 0;
    for (int i = 0; i < length; ++i) {
        res += number[length - i - 1] * (int) pow(numeral, i);
    }
    return res;

}

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
    while(1){
        c = (char)fgetc(input);
        if (c == EOF){
            return EOF;
        }
        c = (char)tolower(c);
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')){
            break;
        }
    }
    fseek(input, -1, SEEK_CUR);
    return c;
}

char* number_finder(FILE* input){
    char* name = (char*) calloc(300, sizeof(char));
    char c;
    int i = 0;
    while(1){
        c = (char)fgetc(input);
        c = (char)tolower(c);
        if(c == EOF){
            break;
        }
        if((c < '0' || c > '9') && (c < 'a' || c > 'z')) {
            fseek(input, ftell(input)-1, SEEK_SET);
            break;
        }
        name[i] = c;
        i++;
    }
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
        printf("Wrong format! Input format: ./[program] [input file] [output file]\n");
        return WRONG_FORMAT;
    }
    if (file_extension(argv[1]) == OK && file_extension(argv[2]) == OK){
        return OK;
    }
    printf("Non .txt file given!\n");
    return NON_TXT;
}

CODE_RESULT action(char** argv){
    FILE *input = fopen(PATH_transform(argv[1]), "r");
    FILE *output = fopen(PATH_transform(argv[2]), "w");
    if (input == NULL){
        return INPUT_FILE_ERROR;
    }
    if (output == NULL){
        return OUTPUT_FILE_ERROR;
    }
    while (1){
        char* number = number_finder(input);
        fprintf(output, "%s %d %d\n", number, numeral_system(number), converter(number, numeral_system(number)));
        free(number);
        if (trash_skipper(input) == EOF)
            break;
    }
    fclose(input);
    fclose(output);
    printf("DONE!\n");
    return OK;
}

int main(int argc, char** argv){
    if (format_validation(argc, argv) == OK){
        action(argv);
    }
    return 0;
}
