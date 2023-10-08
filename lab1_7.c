#include <malloc.h>
#include <ctype.h>
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

int converter(int lexem, int base){
    int res = 0;
    while(lexem > 0){
        res = res * 10 + lexem % base;
        lexem /= base;
    }
    return res;
}

void ASCII_4(char* lexem, FILE* output){
    int length = (int)strlen(lexem);
    int out[length];
    for (int i = 0; i < length; ++i) {
        lexem[i] = (char)tolower(lexem[i]);
        out[i] = converter(lexem[i], 4);
        fprintf(output, "%d", out[i]);
    }
    fputc(' ', output);
}

void ASCII_8(char* lexem, FILE* output){
    int length = (int)strlen(lexem);
    int out[length];
    for (int i = 0; i < length; ++i) {
        out[i] = converter(lexem[i], 8);
        fprintf(output, "%d", out[i]);
    }
    fputc(' ', output);
}

char* all_lower(char* lexem){
    for (int i = 0; i < strlen(lexem); ++i) {
        lexem[i] = (char)tolower(lexem[i]);
    }
    return lexem;
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

CODE_RESULT file_extension(char *name){
    int index = (int)strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.'){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc != 5 && argc != 4){
        printf("Wrong format!\n");
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
                printf("Non .txt file given!\n");
                return NON_TXT;
            }
            printf("Wrong format! Input format for flag '-r': ./[program] -r [file1] [file2] [file3]\n");
            return WRONG_FORMAT;
        }
        else if (letter == 'a'){
            if (argc == 4) {
                if (file_extension(argv[2]) == OK && file_extension(argv[3]) == OK) {
                    return OK;
                }
                printf("Non .txt file given!\n");
                return NON_TXT;
            }
            printf("Wrong format! Input format for flag '-a': ./[program] -a [file1] [file2]\n");
            return WRONG_FORMAT;
        }
        printf("Wrong flag given! Available flags: -a, -r\n");
        return WRONG_FLAG;
    }
    printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
    return NO_FLAG;
}

char* lexem_finder(FILE* input){
    char* name = (char*) calloc(300, sizeof(char));
    char c;
    int i = 0;
    while(1){
        c = (char)fgetc(input);
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
    return name;
}

char trash_skipper(FILE *input){
    char c;
    while(1){
        c = (char)fgetc(input);
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

CODE_RESULT flag_r(char** argv) {
    FILE *input_1 = fopen(PATH_transform(argv[2]), "r");
    FILE *input_2 = fopen(PATH_transform(argv[3]), "r");
    FILE *output = fopen(PATH_transform(argv[4]), "w");
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
            char* lexem = lexem_finder(input_1);
            fprintf(output, "%s ", lexem);
            free(lexem);
            if (trash_skipper(input_1) == EOF){
                file1 = 1;
            }
        }
        else if (i % 2 == 0 && file2 == 0){
            char* lexem = lexem_finder(input_2);
            fprintf(output, "%s ", lexem);
            free(lexem);
            if (trash_skipper(input_2) == EOF){
                file2 = 1;
            }
        }
        else if(file1 == 1 && file2 == 0){
            char* lexem = lexem_finder(input_2);
            fprintf(output, "%s ", lexem);
            free(lexem);
            if (trash_skipper(input_2) == EOF){
                file2 = 1;
            }
        }
        else if(file2 == 1 && file1 == 0){
            char* lexem = lexem_finder(input_1);
            fprintf(output, "%s ", lexem);
            free(lexem);
            if (trash_skipper(input_1) == EOF){
                file1 = 1;
            }
        }
        else{
            break;
        }
        i++;
    }
    fclose(input_1);
    fclose(input_2);
    fclose(output);
    return OK;
}

CODE_RESULT flag_a(char** argv){
    char *PATH_input = PATH_transform(argv[2]);
    char *PATH_output = PATH_transform(argv[3]);
    FILE *input = fopen(PATH_input, "r");
    FILE *output = fopen(PATH_output, "w");
    if (input == NULL){
        return FILE1_ERROR;
    }
    if (output == NULL){
        return FILE2_ERROR;
    }
    int i = 1;
    while(1){
        char* lexem = lexem_finder(input);
        if (i % 10 == 0){
            ASCII_4(lexem, output);
        }
        else if (i % 2 == 0 && i % 10 != 0){
            fprintf(output, "%s ", all_lower(lexem));
        }
        else if (i % 5 == 0 && i % 10 != 0){
            ASCII_8(lexem, output);
        }
        else{
            fprintf(output, "%s ", lexem);
        }
        if(trash_skipper(input) == EOF){
            break;
        }
        free(lexem);
        i++;
    }
    fclose(input);
    fclose(output);
    printf("DONE!\n");
    return OK;
}

void flag_caller(char** argv){
    char flag = argv[1][1];
    switch (flag) {
        case 'a':
            flag_a(argv); break;
        case 'r':
            flag_r(argv); break;
        default:
            printf("Wrong flag given!\n"); break;
    }
}

void greetings(){
    printf("\n\n");
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
        flag_caller(argv);
    }
    return 0;
}