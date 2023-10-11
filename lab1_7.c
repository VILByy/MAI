#include <malloc.h>
#include <ctype.h>
#include "stdio.h"
#include "string.h"

#define MAX_LENGTH 300

typedef enum{
    OK = 1,
    WRONG_FORMAT,
    NON_TXT,
    WRONG_FLAG,
    NO_FLAG,
    FILE1_ERROR,
    FILE2_ERROR,
    FILE3_ERROR,
    MEMORY_ERROR,
}CODE_RESULT;

char letter(long long int number, int base){
    if (number % base >= 10){
        return (char)('A' + (int)(number % base - 10));
    }
    return (char)('0' + (int)(number % base));
}

char* converter(long long int number, int base){
    long long int temp = number;
    int length = 0;
    while(temp > 0){
        temp /= base;
        length++;
    }
    char* res = (char*)calloc(length + 1, sizeof(char));
    if (res == NULL) {
        free(res);
        return NULL;
    }
    for (int i = 0; i < length; ++i) {
        res[length - 1 - i] = letter(number, base);
        number /= base;
    }
    return res;
}

CODE_RESULT ASCII_4(char* lexem, FILE* output){
    int length = (int)strlen(lexem);
    for (int i = 0; i < length; ++i) {
        lexem[i] = (char)tolower(lexem[i]);
        if (converter(lexem[i], 4) == NULL){
            return MEMORY_ERROR;
        }
        fprintf(output, "%s", converter(lexem[i], 4));
    }
    fputc(' ', output);
    return OK;
}

void ASCII_8(char* lexem, FILE* output){
    int length = (int)strlen(lexem);
    for (int i = 0; i < length; ++i) {
        fprintf(output, "%o", (int)lexem[i]);
    }
    fputc(' ', output);
}

char* all_lower(char* lexem){
    for (int i = 0; i < strlen(lexem); ++i) {
        lexem[i] = (char)tolower(lexem[i]);
    }
    return lexem;
}


CODE_RESULT file_extension(char *name){
    int index = (int)strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.'){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc != 5 && argc != 4){
        return WRONG_FORMAT;
    }
    char prefix = argv[1][0];
    char letter = argv[1][1];
    if (prefix == '/' || prefix == '-'){
        if (letter == 'r'){
            if (argc == 5) {
                if (file_extension(argv[2]) == OK && file_extension(argv[3]) == OK && file_extension(argv[4]) == OK) {
                    return OK;
                }
                return NON_TXT;
            }
            return WRONG_FORMAT;
        }
        else if (letter == 'a'){
            if (argc == 4) {
                if (file_extension(argv[2]) == OK && file_extension(argv[3]) == OK) {
                    return OK;
                }
                return NON_TXT;
            }
            return WRONG_FORMAT;
        }
        return WRONG_FLAG;
    }
    return NO_FLAG;
}

char* lexem_finder(FILE* input){
    char* name = (char*) calloc(MAX_LENGTH, sizeof(char));
    if (name == NULL){
        free(name);
        return NULL;
    }
    char c;
    int i = 0, flag = 0;
    while(flag == 0){
        c = (char)fgetc(input);
        if(c == ' ' || c == '\t' || c == '\n') {
            flag = 1;
        }
        if (c == EOF){
            flag = 1;
        }
        name[i] = c;
        i++;
    }
    name[i - 1] = '\0';
    return name;
}

char trash_skipper(FILE *input){
    char c;
    int flag = 0;
    while(flag == 0){
        c = (char)fgetc(input);
        if (c == EOF){
            return EOF;
        }
        if (c != ' ' && c != '\t' && c != '\n'){
            flag = 1;
        }
    }
    fseek(input, -1, SEEK_CUR);
    return c;
}

CODE_RESULT flag_r(char** argv) {
    FILE *input_1 = fopen(argv[2], "r");
    FILE *input_2 = fopen(argv[3], "r");
    FILE *output = fopen(argv[4], "w");
    if (input_1 == NULL) {
        fclose(input_1);
        fclose(input_2);
        fclose(output);
        return FILE1_ERROR;
    }
    if (input_2 == NULL) {
        fclose(input_1);
        fclose(input_2);
        fclose(output);
        return FILE2_ERROR;
    }
    if (output == NULL) {
        fclose(input_1);
        fclose(input_2);
        fclose(output);
        return FILE3_ERROR;
    }
    int flag = 0;
    while (flag == 0){
        char *lexem;
        if (input_1 != NULL) {
            lexem = lexem_finder(input_1);
            if (lexem == NULL) {
                free(lexem);
                fclose(input_1);
                fclose(input_2);
                fclose(output);
                return MEMORY_ERROR;
            }
            fprintf(output, "%s ", lexem);
            free(lexem);
            if (trash_skipper(input_1) == EOF) {
                input_1 = NULL;
            }
        }
        if (input_2 != NULL) {
            lexem = lexem_finder(input_2);
            if (lexem == NULL) {
                free(lexem);
                fclose(input_1);
                fclose(input_2);
                fclose(output);
                return MEMORY_ERROR;
            }
            fprintf(output, "%s ", lexem);
            free(lexem);
            if (trash_skipper(input_2) == EOF) {
                input_2 = NULL;
            }
        }
        if (input_1 == NULL && input_2 == NULL){
            flag = 1;
        }
    }
    fclose(input_1);
    fclose(input_2);
    fclose(output);
    return OK;
}

CODE_RESULT flag_a(char** argv){
    char *PATH_input = argv[2];
    char *PATH_output = argv[3];
    FILE *input = fopen(PATH_input, "r");
    FILE *output = fopen(PATH_output, "w");
    if (input == NULL){
        fclose(input);
        fclose(output);
        return FILE1_ERROR;
    }
    if (output == NULL){
        fclose(input);
        fclose(output);
        return FILE2_ERROR;
    }
    int i = 1, flag = 0;
    while(flag == 0){
        char* lexem = lexem_finder(input);
        if (i % 10 == 0){
            ASCII_4(lexem, output);
            free(lexem);
        }
        else if (i % 2 == 0 && i % 10 != 0){
            fprintf(output, "%s ", all_lower(lexem));
            free(lexem);
        }
        else if (i % 5 == 0){
            ASCII_8(lexem, output);
            free(lexem);
        }
        else{
            fprintf(output, "%s ", lexem);
            free(lexem);
        }
        if(trash_skipper(input) == EOF){
            flag = 1;
        }
        i++;
    }
    fclose(input);
    fclose(output);
    return OK;
}

CODE_RESULT flag_caller(char** argv){
    char flag = argv[1][1];
    switch (flag) {
        case 'a':
            flag_a(argv); break;
        case 'r':
            flag_r(argv); break;
        default:
            return WRONG_FLAG;
    }
    return OK;
}

void status(CODE_RESULT flag){
    switch (flag) {
        case WRONG_FLAG:
            printf("Wrong flag given! Available flags: -a, -r\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case NO_FLAG:
            printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
        case FILE1_ERROR:
            printf("Unable to open FILE1!\n"); break;
        case FILE2_ERROR:
            printf("Unable to open FILE2!\n"); break;
        case FILE3_ERROR:
            printf("Unable to open FILE3!\n"); break;
        case NON_TXT:
            printf("Non txt file given!\n"); break;
        case MEMORY_ERROR:
            printf("Memory error!\n"); break;
        case OK:
            printf("Done!\n"); break;
    }
}

void greetings(){
    printf("---------------------------------Available flags: -a, -r--------------------------------\n");
    printf("-------------------------You have to use '/' or '-' before flag-------------------------\n");
    printf("--------------------------------------Flag info-----------------------------------------\n");
    printf("--------'r' puts in output file lexems one by one from file1 and file2------------------\n");
    printf("----Input format for 'r': ./[program] [key] [input1_path] [input2_path] [output_path]---\n");
    printf("----------'a' puts lexems from file1 to output file following this rules:---------------\n");
    printf("-----1) For every 10th lexem writes symbol`s lowercase ASCII code in quaternary---------\n");
    printf("----------2) For every 2nd (and non-10th) lexem writes this lexem in lower case---------\n");
    printf("-----3) For every 5th (and non-10th) lexem writes symbol`s ASCII code in octal----------\n");
    printf("-----------------In other cases lexem copies to output file-----------------------------\n");
    printf("------------Input format for 'a': ./[program] [key] [input1_path] [output_path]---------\n");
    printf("\n\n");
}

int main(int argc, char** argv){
    greetings();
    if (format_validation(argc, argv) == OK){
        status(flag_caller(argv));
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}