#include "stdio.h"
#include "string.h"

typedef enum{
    OK = 1,
    INPUT_FILE_ERROR,
    WRONG_FORMAT,
    DIF_EXT,
}CODE_RESULT;

void status (CODE_RESULT flag){
    switch (flag) {
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case DIF_EXT:
            printf("Files must be same extension!\n"); break;
        case INPUT_FILE_ERROR:
            printf("Input file error!\n"); break;
        case OK:
            printf("Done!\n"); break;
    }
}

CODE_RESULT format_validation(int argc, char** argv){
    if (argc != 3){
        return WRONG_FORMAT;
    }

    if (strcmp(strrchr(argv[1], '.'), strrchr(argv[2], '.')) != 0){
        return DIF_EXT;
    }
    return OK;
}

CODE_RESULT action(char **argv){
    FILE *input = fopen(argv[1], "r");
    if (input == NULL){
        return INPUT_FILE_ERROR;
    }
    FILE *output = fopen(argv[2], "w");
    char c;
    while ((c = (char)fgetc(input)) != EOF){
        fputc(c, output);
    }
    fclose(input);
    fclose(output);
    return OK;
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