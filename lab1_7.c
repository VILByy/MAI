#include <malloc.h>
#include "stdio.h"
#include "string.h"

typedef enum{
    OK = 1,
    WRONG_FORMAT,
    NON_TXT,
    WRONG_FLAG,
    NO_FLAG,
    FILE1_ERROR,
    FILE2_ERROR,
    FILE3_ERROR,
}CODE_RESULT;

char* PATH_transform(char* PATH){
    int length = strlen(PATH);
    for (int i = 0; i < length; ++i) {
        if (PATH[i] == '\\'){
            PATH[i] = '/';
        }
    }
    return PATH;
}

CODE_RESULT file_extension(char *name){
    int index = strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.'){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc != 5){
        printf("Wrong format!\n");
        return WRONG_FORMAT;
    }
    char prefix = argv[1][0];
    char letter = argv[1][1];
    if (prefix == '/' || prefix == '-'){
        if (letter == 'r' || letter == 'a'){
            if (file_extension(argv[2]) == OK && file_extension(argv[3]) == OK && file_extension(argv[4]) == OK){
                return OK;
            }
            printf("Non .txt file given!\n");
            return NON_TXT;
        }
        printf("Wrong flag given! Available flags: -a, -r\n");
        return WRONG_FLAG;
    }
    printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
    return NO_FLAG;
}

void lexem_printer(FILE* input, FILE* output){
    char* name = (char*) calloc(300, sizeof(char));
    char c;
    int i = 0;
    while(1){
        c = fgetc(input);
        if(c == ' ' || c == '\t' || c == '\n') {
            fseek(input, ftell(input)-1, SEEK_SET);
            break;
        }
        if (c == EOF){
            break;
        }
        name[i] = c;
        i++;
    }
    fprintf(output, "%s ", name);
    free(name);
}

char trash_skipper(FILE *input){
    char c;
    while(1){
        c = fgetc(input);
        if (c == EOF){
            return EOF;
        }
        if (c != ' ' && c != '\t' && c != '\n'){
            break;
        }
    }
    fseek(input, -1, SEEK_CUR);
    return c;
}

//TODO: When 1st file ends, 2nd works for only 2 iterations

CODE_RESULT action(char** argv) {
    char *PATH_input_1 = PATH_transform(argv[2]);
    char *PATH_input_2 = PATH_transform(argv[3]);
    char *PATH_output = PATH_transform(argv[4]);
    FILE *input_1 = fopen(PATH_input_1, "r");
    FILE *input_2 = fopen(PATH_input_2, "r");
    FILE *output = fopen(PATH_output, "w");
    if (input_1 == NULL) {
        return FILE1_ERROR;
    }
    if (input_2 == NULL) {
        return FILE2_ERROR;
    }
    if (output == NULL) {
        return FILE3_ERROR;
    }
    int i = 1, file1 = 0, file2 = 0;
    while (1){
        if (i % 2 != 0 && file1 == 0){
            lexem_printer(input_1, output);
            if (trash_skipper(input_1) == EOF){
                file1 = 1;
            }
        }
        else if (i % 2 == 0 && file2 == 0){
            lexem_printer(input_2, output);
            if (trash_skipper(input_2) == EOF){
                file2 = 1;
            }
        }
        else if(file1 == 1 && file2 == 0){
            lexem_printer(input_2, output);
            if (trash_skipper(input_2) == EOF){
                file2 = 1;
            }
        }
        else if(file2 == 1 && file1 == 0){
            lexem_printer(input_1, output);
            if (trash_skipper(input_2) == EOF){
                file1 = 1;
            }
        }
        else{
            break;
        }
        i++;
    }
    return OK;
}

int main(int argc, char** argv){
    if (format_validation(argc, argv) == OK){
        action(argv);
    }
    return 0;
}