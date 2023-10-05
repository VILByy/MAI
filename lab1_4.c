#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "malloc.h"

typedef enum{
    OK = 1,
    WRONG_FLAG,
    NAN_GIVEN,
    WRONG_FORMAT,
    NO_FLAG,
    FILE_ERROR,
    NON_TXT,
}CODE_RESULT;

char* PATH_transform(char* PATH){
    for (int i = 0; i < strlen(PATH); ++i) {
        if (PATH[i] == '\\'){
            PATH[i] = '/';
        }
    }
    return PATH;
}

char* get_name(char *PATH){
    char* temp = strrchr(PATH, '/');
    char* name = (char*) malloc(sizeof(char) * strlen(temp) + 5);
    name[0] = 'o';
    name[1] = 'u';
    name[2] = 't';
    name[3] = '_';
    for (int i = 0; i < strlen(temp) - 1; ++i) {
        name[i + 4] = temp[i + 1];
    }
    strcpy(temp, name);
    free(name);
    return temp;
}

int islatin(char ch){
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) ? 1 : 0;
}

CODE_RESULT file_extension(char *name){
    int index = strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.'){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT format_validation(int argc, char **argv){
    if (argc < 3 || argc > 4){
        printf("Wrong format!");
        return WRONG_FORMAT;
    }
    char prefix = argv[1][0];
    if (prefix == '/' || prefix == '-'){
        if (strlen(argv[1]) == 3){
            char letter = argv[1][2];
            if (argc == 4) {
                if (argv[1][1] == 'n' && (letter == 'd' || letter == 'i' || letter == 's' || letter == 'a')) {
                    if (file_extension(argv[2]) == OK && file_extension(argv[3]) == OK) {
                        return OK;
                    }
                    printf("Non txt file given!\n");
                    return NON_TXT;
                }
                printf("Flag is not valid! Possible flags: -nd, -ni, -ns, -na, -d, -i, -s, -a.\n");
                return WRONG_FLAG;
            }
            printf("Wrong format!");
            return WRONG_FORMAT;
        }
        if (strlen(argv[1]) == 2){
            char letter = argv[1][1];
            if (argc == 3) {
                if ((letter == 'd' || letter == 'i' || letter == 's' || letter == 'a')) {
                    if (file_extension(argv[2]) == OK) {
                        return OK;
                    }
                    printf("Non txt file given!\n");
                    return NON_TXT;
                }
                printf("Flag is not valid! Possible flags: -nd, -ni, -ns, -na, -d, -i, -s, -a.\n");
                return WRONG_FLAG;
            }
            printf("Wrong format!");
            return WRONG_FORMAT;
        }
    }
    printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
    return NO_FLAG;
}

CODE_RESULT flag_d(FILE *input, FILE *output){
    char ch;
    while ((ch = fgetc(input)) != EOF){
        if(!isdigit(ch))
            fputc(ch, output);
    }
    return OK;
}

CODE_RESULT flag_i(FILE *input, FILE *output){
    char ch;
    int counter = 0;
    char out[300];
    while ((ch = fgetc(input)) != EOF) {
        if (islatin(ch)){
            counter++;
        }
        if (ch == '\n'){
            itoa(counter, out, 10);
            fputs(out, output);
            fputc('\n', output);
            counter = 0;
        }
    }
    itoa(counter, out, 10);
    fputs(out, output);
    return OK;
}

CODE_RESULT flag_s(FILE *input, FILE *output){
    char ch;
    int counter = 0;
    char out[300];
    while ((ch = fgetc(input)) != EOF) {
        if (!islatin(ch) && !isdigit(ch) && ch != ' '){
            counter++;
        }
        if (ch == '\n'){
            itoa(counter, out, 10);
            fputs(out, output);
            fputc('\n', output);
            counter = 0;
        }
    }
    itoa(counter, out, 10);
    fputs(out, output);
    return OK;
}

CODE_RESULT flag_a(FILE *input, FILE *output){
    char ch;
    char out[300];
    while ((ch = fgetc(input)) != EOF){
        if(!isdigit(ch)) {
            itoa(ch, out, 16);
            fputs(out, output);
        }
        else{
            fputc(ch, output);
        }
    }
    return OK;
}

void flag_caller(char** argv){
    char flag = argv[1][strlen(argv[1]) - 1];
    char* PATH_input = PATH_transform(argv[2]);
    FILE *input;
    FILE *output;
    input = fopen(PATH_input, "r");
    int n_status;
    n_status = strlen(argv[1]) == 3 ? 1 : 0;
    if (n_status == 0) {
        output = fopen(get_name(PATH_input), "w");
    } else {
        output = fopen(PATH_transform(argv[3]), "w");
    }
    switch (flag) {
        case 'd':
            flag_d(input, output); break;
        case 'i':
            flag_i(input, output); break;
        case 's':
            flag_s(input, output); break;
        case 'a':
            flag_a(input, output); break;
        default:
            printf("Wrong flag given!"); break;
    }
    fclose(input);
    fclose(output);
}

int main(int argc, char **argv){
    if(format_validation(argc, argv) == OK){
        flag_caller(argv);
    }
}