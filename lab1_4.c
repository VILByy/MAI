#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "malloc.h"

typedef enum{
    OK = 1,
    WRONG_FLAG,
    WRONG_FORMAT,
    NO_FLAG,
    I_FILE_ERROR,
    O_FILE_ERROR,
    NON_TXT,
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

char* get_name(char *PATH){
    char* temp = strrchr(PATH, '/');
    char* name = (char*) malloc(sizeof(char) * strlen(temp) + 5);
    if (name == NULL){
        return NULL;
    }
    name[0] = 'o';
    name[1] = 'u';
    name[2] = 't';
    name[3] = '_';
    if (temp == NULL){
        for (int i = 0; i < strlen(PATH); ++i) {
            name[i + 4] = temp[i];
        }
    }
    else {
        for (int i = 0; i < strlen(temp) - 1; ++i) {
            name[i + 4] = temp[i + 1];
        }
    }
    return name;
}


CODE_RESULT file_extension(char *name){
    int index = (int)strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.'){
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT format_validation(int argc, char **argv){
    if (argc < 3 || argc > 4){
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
                    return NON_TXT;
                }
                return WRONG_FLAG;
            }
            return WRONG_FORMAT;
        }
        if (strlen(argv[1]) == 2){
            char letter = argv[1][1];
            if (argc == 3) {
                if ((letter == 'd' || letter == 'i' || letter == 's' || letter == 'a')) {
                    if (file_extension(argv[2]) == OK) {
                        return OK;
                    }
                    return NON_TXT;
                }
                return WRONG_FLAG;
            }
            return WRONG_FORMAT;
        }
    }
    return NO_FLAG;
}

CODE_RESULT flag_d(FILE *input, FILE *output){
    char ch;
    while ((ch = (char)fgetc(input)) != EOF){
        if(!isdigit(ch))
            fputc(ch, output);
    }
    return OK;
}

CODE_RESULT flag_i(FILE *input, FILE *output){
    char ch;
    int counter = 0;
    while ((ch = (char)fgetc(input)) != EOF) {
        if (isalpha(ch)){
            counter++;
        }
        if (ch == '\n'){
            fprintf(output, "%d\n", counter);
            counter = 0;
        }
    }
    fprintf(output, "%d\n", counter);
    return OK;
}

CODE_RESULT flag_s(FILE *input, FILE *output){
    char ch;
    int counter = 0;
    while ((ch = (char)fgetc(input)) != EOF) {
        if (!isalpha(ch) && !isdigit(ch) && ch != ' '){
            counter++;
        }
        if (ch == '\n'){
            fprintf(output, "%d\n", counter);
            counter = 0;
        }
    }
    fprintf(output, "%d\n", counter);
    return OK;
}

CODE_RESULT flag_a(FILE *input, FILE *output){
    char ch;
    while ((ch = (char)fgetc(input)) != EOF){
        if(!isdigit(ch)) {
            fprintf(output, "%x\n", ch);
        }
        else{
            fputc(ch, output);
        }
    }
    return OK;
}


CODE_RESULT flag_caller(char** argv){
    char flag = argv[1][strlen(argv[1]) - 1];
    char* PATH_input = PATH_transform(argv[2]);
    FILE *input;
    FILE *output;
    input = fopen(PATH_input, "r");
    int n_status;
    n_status = strlen(argv[1]) == 3 ? 1 : 0;
    char *name = get_name(PATH_input);
    if(name == NULL){
        free(name);
        fclose(input);
        return MEMORY_ERROR;
    }
    if (n_status == 0) {
        output = fopen(name, "w");
    } else {
        output = fopen(PATH_transform(argv[3]), "w");
    }
    if(input == NULL){
        free(name);
        fclose(input);
        fclose(output);
        return I_FILE_ERROR;
    }
    if(output == NULL){
        free(name);
        fclose(input);
        fclose(output);
        return O_FILE_ERROR;
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
            fclose(input);
            fclose(output);
            free(name);
            return WRONG_FLAG;
    }
    fclose(input);
    fclose(output);
    free(name);
    return OK;
}

void status(CODE_RESULT flag){
    switch (flag) {
        case WRONG_FLAG:
            printf("Flag is not valid! Possible flags: -nd, -ni, -ns, -na, -d, -i, -s, -a.\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case NO_FLAG:
            printf("Unable to find a flag! Please, put it after the number with symbol '-' or '/'.\n");
        case I_FILE_ERROR:
            printf("Unable to open input file!\n"); break;
        case O_FILE_ERROR:
            printf("Unable to open output file!\n"); break;
        case NON_TXT:
            printf("Non txt file given!\n"); break;
        case MEMORY_ERROR:
            printf("Memory error!\n"); break;
        case OK:
            printf("Done!\n"); break;
    }
}


void greetings(){
    printf("----------------------------Available flags: -d, -i, -s -a------------------------------\n");
    printf("-------------------------You have to use '/' or '-' before flag-------------------------\n");
    printf("----------------------------You may use option 'n' before flag--------------------------\n");
    printf("--------------------Option 'n' allows you enter path to output file---------------------\n");
    printf("---------Otherwise it will be generated in program's directory with prefix 'out_'-------\n");
    printf("-------Input format for : ./[program] [key] [input_path] [OPTIONAL: output_path]--------\n");
    printf("--------------------------------------Flag info-----------------------------------------\n");
    printf("-----------------------'d' deletes all arabic numerals from file------------------------\n");
    printf("-----------------'i' counts how match latin letters in every string---------------------\n");
    printf("--------'s' counts symbols different from latin letters, digits and spaces--------------\n");
    printf("-------------'a' replaces non-digits symbols to ASCII code in hex system----------------\n");
    printf("\n\n");
}

int main(int argc, char **argv){
    greetings();
    if(format_validation(argc, argv) == OK){
        status(flag_caller(argv));
    }
    else{
        status(format_validation(argc, argv));
    }
    return 0;
}