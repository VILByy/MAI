#include <stdio.h>
#include "string.h"

#define DICT_SIZE 11
#define BUFF_SIZE 4

typedef enum {
    OK = 1,
    WRONG_FORMAT,
    NON_TXT,
    WRITE_ERROR,
    READ_ERROR,
    FILE_ERROR,
} CODE_RESULT;

void status(CODE_RESULT flag) {
    switch (flag) {
        case OK:
            printf("\nDone!\n"); break;
        case WRONG_FORMAT:
            printf("Wrong format!\n"); break;
        case NON_TXT:
            printf("Non .txt file given!\n"); break;
        case WRITE_ERROR:
            printf("Write error!\n"); break;
        case READ_ERROR:
            printf("Read error!\n"); break;
        case FILE_ERROR:
            printf("File error!\n"); break;
    }
}

CODE_RESULT file_extension(char *name) {
    int index = (int) strlen(name) - 1;
    if (name[index] == 't' && name[index - 1] == 'x' && name[index - 2] == 't' && name[index - 3] == '.') {
        return OK;
    }
    return NON_TXT;
}


CODE_RESULT format_validation(int argc, char **argv) {
    if (argc != 2) {
        return WRONG_FORMAT;
    }
    if (file_extension(argv[1]) == OK) {
        return OK;
    }
    return NON_TXT;
}

CODE_RESULT action(char **argv) {
    FILE *file = fopen(argv[1], "w");
    char dict[DICT_SIZE] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    if (fwrite(dict, sizeof(char), DICT_SIZE, file) != DICT_SIZE) {
        fclose(file);
        return WRITE_ERROR;
    }
    fclose(file);
    file = fopen(argv[1], "r");
    if (file == NULL){
        return FILE_ERROR;
    }
    char c;
    while (fread(&c, sizeof(char), 1, file)){
        printf("%x %d %d %d %d %p %p %p %d\n", c, file->_cnt, file->_bufsiz, file->_charbuf, file->_file, file->_ptr, file->_base, file->_tmpfname, file->_flag);
    }
    fclose(file);
    file = fopen(argv[1], "r");
    if (file == NULL){
        return FILE_ERROR;
    }
    char buff[BUFF_SIZE];
    fseek(file, 3, SEEK_SET);
    if (fread(&buff, sizeof(char), BUFF_SIZE, file) != BUFF_SIZE){
        fclose(file);
        return READ_ERROR;
    }
    for (int i = 0; i < BUFF_SIZE; ++i) {
        printf("%x ", buff[i]);
    }
    fclose(file);
    return OK;
}

int main(int argc, char **argv) {
    if (format_validation(argc, argv) == OK) {
        status(action(argv));
    } else {
        status(format_validation(argc, argv));
    }
}
