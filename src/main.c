/**
 * @file main.c
 * @author Zdeněk Vavřička
 * @version 1.0
 */

#include "general_header.h"

VFS *vfs;

/**
 * Vstupní bod programu.
 * @param argc počet vstupních parametrů
 * @param argv vstupní parametry
 * @return EXIT_SUCCESS pokud byl program řádně ukončen
 *         EXIT_FAILURE pokud nebyly řádně zadány parametry
 */
int main(int argc, char **argv) {
    char *filename;
    if (argc == 2) {
        filename = argv[1];

        initialize_vfs(&vfs, filename);

        read_from_console();
    } else {
        printf("Incorrect parameters!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void read_from_console() {
    char *line = NULL;

    while (true) {
        line = get_line();

        if (strlen(line) == 0) {
            continue;
        }

        if (process_command(&vfs, line) == false) {
            break;
        }

        memset(line, 0, LINE_LENGTH);
    }

    free(line);
}

