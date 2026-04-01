/**
 * @file utilities.c
 * @author Zdeněk Vavřička
 * @version 1.0
 */

#include "general_header.h"

bool file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

bool str_equals(char *str1, char *str2) {
    if (!str1 || !str2) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return false;
    }

    if (strcmp(str1, str2) == 0) {
        return true;
    } else {
        return false;
    }
}

bool str_empty(char *str) {
    if (str == NULL || str_equals(str, "") == true || str_equals(str, " ") == true) {
        return true;
    }
    return false;
}

int32_t get_size_from_string(char *string) {
    int32_t number;
    char *units = NULL;

    if (!string) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    if (str_empty(string) == true) {
        printf("Unable to format unspecified size.\n");
        return NUM_RETURN_FAILED;
    }

    errno = 0;
    number = (int32_t) strtol(string, &units, 0);

    if (number == 0 || errno != 0) {
        error_print(STRING_NUMBER_CONVERSION_ERROR);
        return STRING_NUMBER_CONVERSION_ERROR;
    }

    if (strchr(units, 'G')) {
        number *= 1000000000;
    } else if (strchr(units, 'M')) {
        number *= 1000000;
    } else if (strchr(units, 'K')) {
        number *= 1000;
    }

    if (number < MIN_FS_SIZE && number > 0) {
        printf("Size too small. (no data blocks would be created) Minimum size: 12KB.\n");
        return NUM_RETURN_FAILED;
    } else if (number < 0 || number > MAX_FS_SIZE) {
        printf("Size too large. Maximum size: 2GB.\n");
        return NUM_RETURN_FAILED;
    }

    return number;
}

char *get_line() {
    int character, i = 0, length = LINE_LENGTH;
    char *line = (char *) calloc(LINE_LENGTH, sizeof(char));

    if (!line) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    while (true) {
        character = getc(stdin);

        if (character == EOF) {
            error_print(FAILED_TO_READ_ERROR);
            break;
        }

        if (i == length) {
            line = realloc((void *) line, length *= 2);

            if (!line) {
                error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
                return NULL;
            }
        }

        if (character == '\n') {
            break;
        }

        line[i++] = (char) character;
    }

    return line;
}

bool control_format_of_VFS(VFS **vfs) {
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return false;
    }

    if ((*vfs)->is_formatted) {
        return true;
    } else {
        printf("You have to format VFS first.\n");
        return false;
    }
}

char *str_trim_and_split(char *string, char **second_part, char split_char) {
    char *ptr = NULL;
    int count = 0;

    if (!string || !split_char) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return NULL;
    }

    *second_part = (char *) calloc(16, sizeof(char));

    if (!*second_part) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    ptr = strrchr(string, split_char);

    if(!ptr){
        return NULL;
    }

    strcpy(*second_part, ptr + 1);

    for (int i = 0; i < strlen(string); ++i) {
        if ((string + i) != ptr) {
            ++count;
        } else {
            break;
        }
    }

    char *firs_part = (char *) calloc(LINE_LENGTH, sizeof(char));

    if (!firs_part) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    if (string[0] == '/' && count == 0) {
        firs_part[0] = '/';
    } else {
        strncpy(firs_part, string, count);
    }

    return firs_part;
}

char *remove_slash_n(char *line) {
    int c, j = 0;
    int length = strlen(line);

    char *new_line = calloc(length, sizeof(char));

    if (!new_line) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        c = line[i];

        if (c != 10 && c != 13) {
            new_line[j] = line[i];
            j++;
        }
    }

    new_line[j] = '\0';

    return new_line;
}

int get_size_of_input_file(FILE *file) {
    int size = DEFAULT_SIZE_OF_INPUT_FILE;

    if (!file) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return size;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}

void error_print(int error) {
    if (error == FAILED_TO_PASS_PARAMETERS_ERROR) {
        printf("ERROR: Failed to pass function parameters.\n");
        exit(FAILED_TO_PASS_PARAMETERS_ERROR);
    } else if (error == FAILED_TO_ALLOCATE_MEMORY_ERROR) {
        printf("ERROR: Failed to allocate memory.\n");
        exit(FAILED_TO_ALLOCATE_MEMORY_ERROR);
    } else if (error == FAILED_TO_READ_ERROR) {
        printf("ERROR: Failed to debug from console or file.\n");
        exit(FAILED_TO_READ_ERROR);
    } else if (error == FAILED_TO_OPEN_FILE) {
        printf("ERROR: Failed to open file.\n");
        exit(FAILED_TO_OPEN_FILE);
    } else {
        printf("ERROR: General error.\n");
        exit(GENERAL_ERROR);
    }
}