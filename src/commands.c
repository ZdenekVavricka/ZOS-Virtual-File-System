/**
 * @file commands.c
 * @author Zdeněk Vavřička
 * @version 1.0
 */

#include "general_header.h"

int process_command(VFS **vfs, char *line) {
    char *command;

    if (!*line) {
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    command = strtok(line, COMMAND_DELIMETER);

    if (strcmp(command, EXIT) == 0) {
        printf("End of program.\n");
        return false;
    } else if (strcmp(command, CP) == 0) {
        if (control_format_of_VFS(vfs) == true) {
            char *src_filename = strtok(NULL, COMMAND_DELIMETER);
            char *dest_filename = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(src_filename) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            if (str_empty(dest_filename) == true) {
                printf("Destination file not defined.\n");
                return true;
            }

            cp_command(vfs, src_filename, dest_filename);
        }
        return true;
    } else if (str_equals(command, MV) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *src_filename = strtok(NULL, COMMAND_DELIMETER);
            char *dest_filename = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(src_filename) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            if (str_empty(dest_filename) == true) {
                printf("Destination file not defined.\n");
                return true;
            }

            mv_command(vfs, src_filename, dest_filename);
        }
        return true;
    } else if (str_equals(command, RM) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            rm_command(vfs, path);
        }
        return true;

    } else if (str_equals(command, MKDIR) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            mkdir_command(vfs, path);
        }
        return true;
    } else if (str_equals(command, RMDIR) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            rmdir_command(vfs, path);
        }
        return true;
    } else if (str_equals(command, LS) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                path = ".";
            }
            ls_command(vfs, path);
        }
        return true;
    } else if (str_equals(command, CAT) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                printf("Source file not defined.\n");
                return true;
            }
            cat_command(vfs, path);
        }
        return true;
    } else if (str_equals(command, CD) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                printf("Source file not defined.\n");
                return true;
            }
            cd_command(vfs, path);
        }
        return true;
    } else if (str_equals(command, PWD) == true) {
        if (control_format_of_VFS(vfs) == true) {
            pwd_command(vfs);
        }
        return true;
    } else if (str_equals(command, INFO) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *path = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(path) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            info_command(vfs, path);
        }
        return true;
    } else if (str_equals(command, INCP) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *src_filename = strtok(NULL, COMMAND_DELIMETER);
            char *dest_filename = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(src_filename) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            if (str_empty(dest_filename) == true) {
                dest_filename = ".";
            }

            incp_command(vfs, src_filename, dest_filename);

        }
        return true;
    } else if (str_equals(command, OUTCP) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *src_filename = strtok(NULL, COMMAND_DELIMETER);
            char *dest_filename = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(src_filename) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            if (str_empty(dest_filename) == true) {
                printf("Destination file not defined.\n");
                return true;
            }

            outcp_command(vfs, src_filename, dest_filename);

        }
        return true;
    } else if (str_equals(command, LOAD) == true) {
        char *path = strtok(NULL, COMMAND_DELIMETER);

        if (str_empty(path) == true) {
            printf(FILE_NOT_FOUND);
            return true;
        }

        load_command(vfs, path);

        return true;
    } else if (str_equals(command, FORMAT) == true) {
        command = strtok(NULL, COMMAND_DELIMETER);

        if (str_empty(command) == true) {
            return true;
        }

        int32_t filesystem_size = get_size_from_string(command);

        if (filesystem_size == NUM_RETURN_FAILED) {
            return true;
        }

        if ((*vfs)->is_formatted == true) {
            free((*vfs)->super_block);
            free((*vfs)->inode_bitmap);
            free((*vfs)->data_bitmap);
            free((*vfs)->current);
            format_command(vfs, filesystem_size);
            return true;
        } else {
            format_command(vfs, filesystem_size);
            return true;
        }

    } else if (str_equals(command, SLINK) == true) {
        if (control_format_of_VFS(vfs) == true) {
            char *src_filename = strtok(NULL, COMMAND_DELIMETER);
            char *dest_filename = strtok(NULL, COMMAND_DELIMETER);

            if (str_empty(src_filename) == true) {
                printf("Source file not defined.\n");
                return true;
            }

            if (str_empty(dest_filename) == true) {
                printf("Destination file not defined.\n");
                return true;
            }

            slink_command(vfs, src_filename, dest_filename);

        }
        return true;
    } else if (str_equals(command, DEBUG) == true) {
        if (control_format_of_VFS(vfs) == true) {
            debug_command(vfs);
        }
        return true;
    } else {
        printf("Unknown command: %s\n", command);
        return true;
    }
}

void cp_command(VFS **vfs, char *src_path, char *dest_path) {
    int id, file_size, count_data_blocks, num_of_free_blocks;
    directory_item *src_dir, *dest_dir;
    char *dest_file_name, *des_path;

    if (!vfs || !*vfs || !src_path || !dest_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(src_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    if (str_empty(dest_path) == true) {
        printf(PATH_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (strchr(dest_path, '/') != NULL) {
        des_path = str_trim_and_split(dest_path, &dest_file_name, '/');

        if (!des_path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(dest_file_name, ".") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dest_file_name, "..") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dest_dir = find_directory_item_by_path(vfs, des_path);

        if (!dest_dir) {
            printf(PATH_NOT_FOUND);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        dest_file_name = dest_path;

        if (str_equals(dest_file_name, ".") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dest_file_name, "..") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dest_dir = calloc(1, sizeof(directory_item));

        if (!dest_dir) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(dest_dir, (*vfs)->current, sizeof(directory_item));
    }

    if (strlen(dest_file_name) > (FILENAME_LENGTH - 1)) {
        printf("File name too long.\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    src_dir = find_directory_item_by_path(vfs, src_path);

    if (!src_dir) {
        printf(FILE_NOT_FOUND);
        return;
    }

    directory_item *temp = find_directory_item_by_name(vfs, dest_dir, dest_file_name);

    if (temp) {
        fclose((*vfs)->vfs_file);
        free(src_dir);
        free(temp);
        printf(EXIST);
        return;
    }

    inode *new_inode = (inode *) calloc(1, sizeof(inode));

    if (!new_inode) {
        free(new_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    inode *src_inode = load_inode(vfs, src_dir->inode);

    if (src_inode->type_of_file == DIRECTORY_TYPE) {
        printf("The command can only be done with files.\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    file_size = src_inode->file_size;

    count_data_blocks = file_size / CLUSTER_SIZE;

    if (file_size % CLUSTER_SIZE != 0) {
        ++count_data_blocks;
    }

    inode *dest_inode = load_inode(vfs, dest_dir->inode);

    if ((dest_inode->file_size + file_size) > MAX_FILE_SIZE) {
        printf("Maximum file size exceeded.\n");
        free(new_inode);
        free(src_inode);
        free(dest_inode);
        free(src_dir);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    num_of_free_blocks = num_free_data_blocks((*vfs)->data_bitmap, ((*vfs)->super_block->cluster_count / 8));

    if (num_of_free_blocks == LACK_OF_FREE_ELEMENTS) {
        printf("No free blocks.\n");
        free(new_inode);
        free(src_inode);
        free(dest_inode);
        free(src_dir);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    if (num_of_free_blocks >= count_data_blocks) {
        id = find_free_inode(vfs);

        if (id > 0) {
            change_value_of_bitmap_element((*vfs)->inode_bitmap, id);

            new_inode->node_id = id;
            new_inode->type_of_file = src_inode->type_of_file;
            new_inode->references = 1;
            new_inode->file_size = file_size;

            for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
                new_inode->direct[i] = ID_ITEM_FREE;
            }

            new_inode->indirect1 = ID_ITEM_FREE;
            new_inode->indirect2 = ID_ITEM_FREE;

            write_inode_to_vfs(vfs, new_inode);

            directory_item *dir_item = new_directory_item(new_inode->node_id, dest_file_name);

            write_directory_item(vfs, dest_dir, dir_item);

            copy_file_data_blocks(vfs, src_dir, dir_item);

            write_bitmap_to_vfs(vfs, true);

            write_bitmap_to_vfs(vfs, false);

            free(dir_item);
        } else {
            printf("No free inodes.\n");
            free(new_inode);
            free(dest_inode);
            free(src_dir);
            free(temp);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        printf("Lack of free blocks.\n");
        free(new_inode);
        free(dest_inode);
        free(src_dir);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    free(new_inode);
    free(dest_inode);
    free(src_dir);
    free(temp);

    printf(OK);

    fclose((*vfs)->vfs_file);
}

void mv_command(VFS **vfs, char *src_path, char *dest_path) {
    int num_of_free_blocks;
    directory_item *src_directory, *dest_directory;
    char *src_file_name, *dest_file_name, *source_path, *destination_path, *name;

    if (!vfs || !*vfs || !src_path || !dest_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(src_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    if (str_empty(dest_path) == true) {
        printf(PATH_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (strchr(src_path, '/') != NULL) {
        source_path = str_trim_and_split(src_path, &src_file_name, '/');

        if (!source_path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(src_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(src_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        src_directory = find_directory_item_by_path(vfs, source_path);

        if (!src_directory) {
            printf(FILE_NOT_FOUND);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        src_file_name = src_path;

        if (str_equals(src_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(src_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        src_directory = calloc(1, sizeof(directory_item));

        if (!src_directory) {
            fclose((*vfs)->vfs_file);
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(src_directory, (*vfs)->current, sizeof(directory_item));
    }

    if (strchr(dest_path, '/') != NULL) {
        destination_path = str_trim_and_split(dest_path, &dest_file_name, '/');

        if (!destination_path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(dest_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dest_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dest_directory = find_directory_item_by_path(vfs, destination_path);

        if (!dest_directory) {
            printf(PATH_NOT_FOUND);
            free(src_directory);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        dest_file_name = dest_path;

        if (str_equals(dest_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dest_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dest_directory = calloc(1, sizeof(directory_item));

        if (!dest_directory) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(dest_directory, (*vfs)->current, sizeof(directory_item));
    }

    if (strlen(dest_file_name) > (FILENAME_LENGTH - 1)) {
        printf("File name too long.\n");
        free(src_directory);
        free(dest_directory);
        fclose((*vfs)->vfs_file);
        return;
    }

    directory_item *src_dir = find_directory_item_by_name(vfs, src_directory, src_file_name);

    if (!src_dir) {
        printf(FILE_NOT_FOUND);
        free(src_directory);
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *src_dir_inode = load_inode(vfs, src_dir->inode);

    if (src_dir_inode->type_of_file == DIRECTORY_TYPE) {
        printf("The command can only be done with files.\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    free(src_dir_inode);

    directory_item *dest_dir = find_directory_item_by_name(vfs, dest_directory, dest_file_name);

    if (dest_dir) {
        inode *dest_inode = load_inode(vfs, dest_dir->inode);

        if (dest_inode->type_of_file == DIRECTORY_TYPE) {
            free(dest_directory);
            dest_directory = find_directory_item_by_path(vfs, dest_path);

            if (!dest_directory) {
                printf(PATH_NOT_FOUND);
                free(src_directory);
                free(src_dir);
                free(src_dir_inode);
                free(dest_dir);
                fclose((*vfs)->vfs_file);
                return;
            }
        }

        free(dest_inode);
    }

    if (src_directory->inode == dest_directory->inode &&
        str_equals(src_directory->item_name, dest_directory->item_name) == true) {
        directory_item *temp = find_directory_item_by_name(vfs, dest_directory, dest_file_name);

        if (temp) {
            printf(EXIST);
            free(dest_directory);
            free(src_directory);
            free(temp);
            fclose((*vfs)->vfs_file);
            return;
        }

        name = (char *) calloc(FILENAME_LENGTH, sizeof(char));

        if (!name) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        strncpy(name, src_dir->item_name, FILENAME_LENGTH);

        char buff[FILENAME_LENGTH] = {'\0'};

        strncpy(buff, dest_file_name, strlen(dest_file_name));

        strncpy(src_dir->item_name, buff, FILENAME_LENGTH);

        rewrite_directory_item(vfs, src_directory, name, src_dir);

        free(name);
        free(temp);
    } else {
        directory_item *temp = find_directory_item_by_name(vfs, dest_directory, src_file_name);

        if (temp) {
            fclose((*vfs)->vfs_file);
            free(dest_directory);
            free(src_directory);
            free(temp);
            printf(EXIST);
            return;
        }

        inode *src_inode = load_inode(vfs, src_directory->inode);

        inode *dest_inode = load_inode(vfs, dest_directory->inode);

        if ((dest_inode->file_size + DIRECTORY_ITEM_SIZE) > MAX_FILE_SIZE) {
            printf("Maximum file size exceeded.\n");
            free(dest_inode);
            free(src_directory);
            fclose((*vfs)->vfs_file);
            return;
        }

        num_of_free_blocks = num_free_data_blocks((*vfs)->data_bitmap, ((*vfs)->super_block->cluster_count / 8));

        if (num_of_free_blocks == LACK_OF_FREE_ELEMENTS) {
            printf("No free blocks.\n");
            free(dest_inode);
            free(src_directory);
            fclose((*vfs)->vfs_file);
            return;
        }

        write_directory_item(vfs, dest_directory, src_dir);

        delete_dir_item(vfs, src_directory, src_dir);

        src_inode->file_size -= DIRECTORY_ITEM_SIZE;

        write_inode_to_vfs(vfs, src_inode);

        return_data_blocks(vfs, src_directory);

        free(temp);
        free(src_inode);
        free(dest_inode);
    }

    printf(OK);

    free(dest_directory);
    free(src_directory);

    fclose((*vfs)->vfs_file);
}

void rm_command(VFS **vfs, char *string_path) {
    char *path;
    directory_item *dir;
    char *dir_name;

    if (!vfs || !*vfs || !string_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(string_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (strchr(string_path, '/') != NULL) {
        path = str_trim_and_split(string_path, &dir_name, '/');

        if (!path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dir = find_directory_item_by_path(vfs, path);

        if (!dir) {
            printf(FILE_NOT_FOUND);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        dir_name = string_path;

        dir = calloc(1, sizeof(directory_item));

        if (!dir) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(dir, (*vfs)->current, sizeof(directory_item));
    }

    directory_item *file_to_remove = find_directory_item_by_name(vfs, dir, dir_name);

    if (!file_to_remove) {
        printf(FILE_NOT_FOUND);
        free(dir);
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *inode_of_remove = load_inode(vfs, file_to_remove->inode);

    inode *current_inode = load_inode(vfs, dir->inode);

    if (inode_of_remove->type_of_file != DIRECTORY_TYPE) {
        free_all_file_data_blocks(vfs, file_to_remove);

        delete_dir_item(vfs, dir, file_to_remove);

        change_value_of_bitmap_element((*vfs)->inode_bitmap, inode_of_remove->node_id);

        write_bitmap_to_vfs(vfs, true);

        set_inode_to_free(vfs, inode_of_remove);

        current_inode->file_size -= DIRECTORY_ITEM_SIZE;

        write_inode_to_vfs(vfs, current_inode);

        return_data_blocks(vfs, dir);

    } else {
        printf("The command can only be done with files.\n");
        free(dir);
        free(file_to_remove);
        free(inode_of_remove);
        free(current_inode);
        return;
    }

    printf(OK);

    free(dir);
    free(file_to_remove);
    free(inode_of_remove);
    free(current_inode);

    fclose((*vfs)->vfs_file);
}

void mkdir_command(VFS **vfs, char *string_path) {
    int id, num_of_free_blocks;
    char *path;
    directory_item *dir;
    char *dir_name;

    if (!vfs || !*vfs || !string_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(string_path) == true) {
        printf(PATH_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    inode *new_inode = (inode *) calloc(1, sizeof(inode));

    if (!new_inode) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    if (strchr(string_path, '/') != NULL) {
        path = str_trim_and_split(string_path, &dir_name, '/');

        if (!path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(dir_name, ".") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dir_name, "..") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (strlen(dir_name) > (FILENAME_LENGTH - 1)) {
            printf("File name too long.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dir = find_directory_item_by_path(vfs, path);

        if (!dir) {
            printf(PATH_NOT_FOUND);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        if (strlen(string_path) > (FILENAME_LENGTH - 1)) {
            printf("File name too long.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dir_name = string_path;

        if (str_equals(dir_name, ".") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dir_name, "..") == true) {
            printf("File with this name cannot be created.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dir = calloc(1, sizeof(directory_item));

        if (!dir) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(dir, (*vfs)->current, sizeof(directory_item));
    }

    directory_item *temp = find_directory_item_by_name(vfs, dir, dir_name);

    if (temp) {
        printf(EXIST);
        free(path);
        free(dir);
        free(new_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *current_inode = load_inode(vfs, dir->inode);

    if (current_inode->type_of_file != DIRECTORY_TYPE){
        printf("Destination is not a folder.\n");
        free(path);
        free(dir);
        free(new_inode);
        free(temp);
        return;
    }

    if ((current_inode->file_size + DIRECTORY_ITEM_SIZE) > MAX_FILE_SIZE) {
        printf("Maximum file size exceeded.\n");
        free(path);
        free(dir);
        free(new_inode);
        free(current_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    num_of_free_blocks = num_free_data_blocks((*vfs)->data_bitmap, ((*vfs)->super_block->cluster_count / 8));

    if (num_of_free_blocks == LACK_OF_FREE_ELEMENTS) {
        printf("No free blocks.\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    if (num_of_free_blocks >= 1) {
        id = find_free_inode(vfs);

        if (id > 0) {
            change_value_of_bitmap_element((*vfs)->inode_bitmap, id);

            new_inode->node_id = id;
            new_inode->type_of_file = DIRECTORY_TYPE;
            new_inode->references = 1;
            new_inode->file_size = 0;

            for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
                new_inode->direct[i] = ID_ITEM_FREE;
            }

            new_inode->indirect1 = ID_ITEM_FREE;
            new_inode->indirect2 = ID_ITEM_FREE;

            write_inode_to_vfs(vfs, new_inode);

            directory_item *dir_item = new_directory_item(new_inode->node_id, dir_name);

            write_directory_item(vfs, dir, dir_item);

            directory_item *same_dir = new_directory_item(new_inode->node_id, ".");

            write_directory_item(vfs, dir_item, same_dir);

            directory_item *parent = new_directory_item(dir->inode, "..");

            write_directory_item(vfs, dir_item, parent);

            write_bitmap_to_vfs(vfs, true);

            write_bitmap_to_vfs(vfs, false);

            free(dir_item);
            free(same_dir);
            free(parent);

        } else {
            printf("No free inodes.\n");
            free(path);
            free(dir);
            free(new_inode);
            free(current_inode);
            free(temp);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        printf("Lack of free blocks.\n");
        free(path);
        free(dir);
        free(new_inode);
        free(current_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    free(path);
    free(new_inode);
    free(current_inode);
    free(temp);

    printf(OK);

    fclose((*vfs)->vfs_file);
}

void rmdir_command(VFS **vfs, char *string_path) {
    char *path;
    int8_t delete_buffer[CLUSTER_SIZE] = {0};
    directory_item *dir;
    char *dir_name;

    if (!vfs || !*vfs || !string_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(string_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (strchr(string_path, '/') != NULL) {
        path = str_trim_and_split(string_path, &dir_name, '/');

        if (!path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(dir_name, ".") == true) {
            printf("Non-removable file.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dir_name, "..") == true) {
            printf("Non-removable file.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dir = find_directory_item_by_path(vfs, path);

        if (!dir) {
            printf(FILE_NOT_FOUND);
            free(dir);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        dir_name = string_path;

        if (str_equals(dir_name, ".") == true) {
            printf("Non-removable file.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dir_name, "..") == true) {
            printf("Non-removable file.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        dir = calloc(1, sizeof(directory_item));

        if (!dir) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(dir, (*vfs)->current, sizeof(directory_item));
    }

    directory_item *dir_to_remove = find_directory_item_by_name(vfs, dir, dir_name);

    if (!dir_to_remove) {
        printf(FILE_NOT_FOUND);
        free(dir);
        free(dir_to_remove);
        return;
    }

    inode *inode_of_remove = load_inode(vfs, dir_to_remove->inode);

    inode *current_inode = load_inode(vfs, dir->inode);

    if (inode_of_remove->type_of_file == DIRECTORY_TYPE) {
        if (inode_of_remove->file_size == (2 * DIRECTORY_ITEM_SIZE)) {
            for (int i = 1; i < NUM_OF_DIRECT_LINK; ++i) {
                if (inode_of_remove->direct[i] != ID_ITEM_FREE) {
                    printf(NOT_EMPTY);
                    free(dir);
                    free(dir_to_remove);
                    free(inode_of_remove);
                    free(current_inode);
                    fclose((*vfs)->vfs_file);
                    return;
                }
            }

            if (inode_of_remove->indirect1 != ID_ITEM_FREE) {
                printf(NOT_EMPTY);
                free(dir);
                free(dir_to_remove);
                free(inode_of_remove);
                free(current_inode);
                fclose((*vfs)->vfs_file);
                return;
            }

            if (inode_of_remove->indirect2 != ID_ITEM_FREE) {
                printf(NOT_EMPTY);
                free(dir);
                free(dir_to_remove);
                free(inode_of_remove);
                free(current_inode);
                fclose((*vfs)->vfs_file);
                return;
            }

            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (inode_of_remove->direct[0] * CLUSTER_SIZE)));
            write_vfs(vfs, delete_buffer, sizeof(int8_t), CLUSTER_SIZE);

            change_value_of_bitmap_element((*vfs)->data_bitmap, inode_of_remove->direct[0]);

            write_bitmap_to_vfs(vfs, false);

            delete_dir_item(vfs, dir, dir_to_remove);

            change_value_of_bitmap_element((*vfs)->inode_bitmap, inode_of_remove->node_id);

            write_bitmap_to_vfs(vfs, true);

            set_inode_to_free(vfs, inode_of_remove);

            current_inode->file_size -= DIRECTORY_ITEM_SIZE;

            write_inode_to_vfs(vfs, current_inode);

            return_data_blocks(vfs, dir);
        } else {
            printf(NOT_EMPTY);
            free(dir);
            free(dir_to_remove);
            free(inode_of_remove);
            free(current_inode);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        printf("It is not a folder.\n");
        free(dir);
        free(dir_to_remove);
        free(inode_of_remove);
        free(current_inode);
        fclose((*vfs)->vfs_file);
        return;
    }

    printf(OK);

    free(dir);
    free(dir_to_remove);
    free(inode_of_remove);
    free(current_inode);
    fclose((*vfs)->vfs_file);
}

void ls_command(VFS **vfs, char *path) {
    int test = 0;

    if (!vfs || !*vfs || !path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    directory_item *dir = find_directory_item_by_path(vfs, path);

    if (!dir) {
        printf(PATH_NOT_FOUND);
        fclose((*vfs)->vfs_file);
        return;
    }

    test = print_content_of_directory(vfs, dir);

    if (test == 1) {
        printf("The command can only be done with directory.\n");
    }

    free(dir);

    fclose((*vfs)->vfs_file);
}

void cat_command(VFS **vfs, char *path) {
    int test;
    char *temp_path;

    if (!vfs || !*vfs || !path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (test_slink(vfs, path) == true) {
        temp_path = get_path_from_slink(vfs, path);

        if (!temp_path) {
            printf(BROKEN_LINK);
            fclose((*vfs)->vfs_file);
            return;
        }

        path = temp_path;
    }

    directory_item *dir = find_directory_item_by_path(vfs, path);

    if (!dir) {
        printf(FILE_NOT_FOUND);
        fclose((*vfs)->vfs_file);
        return;
    }

    test = print_content_of_file(vfs, dir);

    if (test == 1) {
        printf("The command can only be done with files.\n");
    }

    free(dir);

    fclose((*vfs)->vfs_file);
}

void cd_command(VFS **vfs, char *path) {
    if (!vfs || !*vfs || !path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    directory_item *final_dir = find_directory_item_by_path(vfs, path);

    if (!final_dir) {
        printf(PATH_NOT_FOUND);
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *final_inode = load_inode(vfs, final_dir->inode);

    if (final_inode->type_of_file != DIRECTORY_TYPE) {
        printf("The command can only be done with directories.\n");
        free(final_inode);
        return;
    }

    (*vfs)->current = final_dir;

    printf(OK);

    free(final_inode);
    fclose((*vfs)->vfs_file);
}

void pwd_command(VFS **vfs) {
    char *path;
    directory_item *current;

    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }


    current = calloc(1, sizeof(directory_item));

    if (!current) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    memcpy(current, (*vfs)->current, sizeof(directory_item));

    path = get_path(vfs, current, LINE_LENGTH);

    printf("PATH: ");

    strcat(path, "\n");

    printf("%s", path);

    free(path);

    fclose((*vfs)->vfs_file);
}

void info_command(VFS **vfs, char *path) {
    if (!vfs || !*vfs || !path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    directory_item *dir = find_directory_item_by_path(vfs, path);

    if (!dir) {
        printf(FILE_NOT_FOUND);
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *current_inode = load_inode(vfs, dir->inode);

    printf("NAME: %s\n", dir->item_name);
    printf("SIZE: %d\n", current_inode->file_size);
    printf("I-NODE NUMBER: %d\n", current_inode->node_id);

    for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
        printf("DIRECT %d: %d\n", i, current_inode->direct[i]);
    }

    printf("INDIRECT 1: %d\n", current_inode->indirect1);
    printf("INDIRECT 2: %d\n", current_inode->indirect2);

    free(dir);
    free(current_inode);

    fclose((*vfs)->vfs_file);
}

void incp_command(VFS **vfs, char *src_path, char *dest_path) {
    int id, file_size, count_data_blocks, num_of_free_blocks;
    directory_item *current_dir;
    char *file_name, *trash;
    int8_t *data;
    FILE *input_file;

    if (!vfs || !*vfs || !src_path || !dest_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(src_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    if (str_empty(dest_path) == true) {
        printf(PATH_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    trash = str_trim_and_split(src_path, &file_name, '/');

    if (!trash){
        printf("The path was probably not entered with a correct slash. (/)\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    if (str_equals(file_name, ".") == true) {
        printf("File with this name cannot be created.\n");
        fclose((*vfs)->vfs_file);
        return;
    } else if (str_equals(file_name, "..") == true) {
        printf("File with this name cannot be created.\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    if (strlen(file_name) > (FILENAME_LENGTH - 1)) {
        printf("File name too long.\n");
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *new_inode = (inode *) calloc(1, sizeof(inode));

    if (!new_inode) {
        free(new_inode);
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }


    current_dir = find_directory_item_by_path(vfs, dest_path);

    if (!current_dir) {
        printf(PATH_NOT_FOUND);
        fclose((*vfs)->vfs_file);
        return;
    }


    directory_item *temp = find_directory_item_by_name(vfs, current_dir, file_name);

    if (temp) {
        fclose((*vfs)->vfs_file);
        free(new_inode);
        free(current_dir);
        free(temp);
        printf(EXIST);
        return;
    }

    data = (int8_t *) calloc(1, CLUSTER_SIZE);

    if (!data) {
        free(new_inode);
        free(current_dir);
        free(temp);
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    if (!file_exists(src_path)) {
        printf(FILE_NOT_FOUND);
        free(data);
        free(new_inode);
        free(current_dir);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    input_file = fopen(src_path, "rb+");

    if (!input_file) {
        printf(FILE_NOT_FOUND);
        free(data);
        free(new_inode);
        free(current_dir);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    file_size = get_size_of_input_file(input_file);

    count_data_blocks = file_size / CLUSTER_SIZE;

    if (file_size % CLUSTER_SIZE != 0) {
        ++count_data_blocks;
    }

    inode *current_inode = load_inode(vfs, current_dir->inode);

    if (current_inode->type_of_file != DIRECTORY_TYPE) {
        printf("The command can only be done with files.\n");
        free(new_inode);
        free(current_inode);
        fclose((*vfs)->vfs_file);
        return;
    }

    if ((current_inode->file_size + DIRECTORY_ITEM_SIZE) > MAX_FILE_SIZE) {
        printf("Maximum file size exceeded.\n");
        free(data);
        free(new_inode);
        free(current_inode);
        free(current_dir);
        free(temp);
        fclose(input_file);
        fclose((*vfs)->vfs_file);
        return;
    }

    if (file_size > MAX_FILE_SIZE) {
        printf("Maximum file size exceeded.\n");
        free(data);
        free(new_inode);
        free(current_inode);
        free(current_dir);
        free(temp);
        fclose(input_file);
        fclose((*vfs)->vfs_file);
        return;
    }

    num_of_free_blocks = num_free_data_blocks((*vfs)->data_bitmap, ((*vfs)->super_block->cluster_count / 8));

    if (num_of_free_blocks == LACK_OF_FREE_ELEMENTS) {
        printf("No free blocks.\n");
        free(data);
        free(new_inode);
        free(current_inode);
        free(current_dir);
        free(temp);
        fclose(input_file);
        fclose((*vfs)->vfs_file);
        return;
    }

    if (num_of_free_blocks >= count_data_blocks) {
        id = find_free_inode(vfs);

        if (id > 0) {
            change_value_of_bitmap_element((*vfs)->inode_bitmap, id);

            new_inode->node_id = id;
            new_inode->type_of_file = FILE_TYPE;
            new_inode->references = 1;
            new_inode->file_size = file_size;

            for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
                new_inode->direct[i] = ID_ITEM_FREE;
            }

            new_inode->indirect1 = ID_ITEM_FREE;
            new_inode->indirect2 = ID_ITEM_FREE;

            write_inode_to_vfs(vfs, new_inode);

            directory_item *dir_item = new_directory_item(new_inode->node_id, file_name);

            write_directory_item(vfs, current_dir, dir_item);

            for (int i = 0; i < count_data_blocks; ++i) {
                fread(data, sizeof(int8_t), CLUSTER_SIZE, input_file);
                write_file(vfs, dir_item, data);
                memset(data, 0, CLUSTER_SIZE);
            }

            write_bitmap_to_vfs(vfs, true);

            write_bitmap_to_vfs(vfs, false);

            free(dir_item);
        } else {
            printf("No free inodes.\n");
            free(data);
            free(new_inode);
            free(current_inode);
            free(current_dir);
            free(temp);
            fclose(input_file);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        printf("Lack of free blocks.\n");
        free(data);
        free(new_inode);
        free(current_inode);
        free(current_dir);
        free(temp);
        fclose(input_file);
        fclose((*vfs)->vfs_file);
        return;
    }

    free(data);
    free(new_inode);
    free(current_inode);
    free(current_dir);
    free(temp);

    printf(OK);

    fclose(input_file);
    fclose((*vfs)->vfs_file);
}

void outcp_command(VFS **vfs, char *src_path, char *dest_path) {
    char *temp, *temp_path;
    directory_item *current_dir;

    FILE *output_file;

    if (!vfs || !*vfs || !src_path || !dest_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(src_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    if (str_empty(dest_path) == true) {
        printf(PATH_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (test_slink(vfs, src_path) == true) {
        temp_path = get_path_from_slink(vfs, src_path);

        if (!temp_path) {
            fclose((*vfs)->vfs_file);
            printf(BROKEN_LINK);
            return;
        }

        src_path = temp_path;
    }


    current_dir = find_directory_item_by_path(vfs, src_path);

    if (!current_dir) {
        printf(FILE_NOT_FOUND);
        fclose((*vfs)->vfs_file);
        return;
    }

    inode *current_inode = load_inode(vfs, current_dir->inode);

    if (current_inode->type_of_file == DIRECTORY_TYPE) {
        printf("The command can only be done with files.\n");
        free(current_inode);
        fclose((*vfs)->vfs_file);
        return;
    }

    temp = (char *) calloc(16, sizeof(char));

    if (!temp) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    temp[0] = '/';

    strcat(temp, current_dir->item_name);

    strcat(dest_path, temp);

    output_file = fopen(dest_path, "wb+");

    if (!output_file) {
        printf(PATH_NOT_FOUND);
        free(current_dir);
        free(temp);
        fclose(output_file);
        fclose((*vfs)->vfs_file);
        return;
    }

    write_out_content_of_file(vfs, current_dir, output_file);

    printf(OK);

    free(temp);
    fclose(output_file);
    fclose((*vfs)->vfs_file);
}

void load_command(VFS **vfs, char *path) {
    FILE *file_commands;
    char input[LINE_LENGTH] = {0};
    char *line;

    if (!vfs || !*vfs || !path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (!file_exists(path)) {
        printf(FILE_NOT_FOUND);
        return;
    }

    file_commands = fopen(path, "r");

    if (!file_commands) {
        fclose(file_commands);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    while (fgets(input, LINE_LENGTH, file_commands)) {
        line = remove_slash_n(input);

        if (strlen(input) == 0) {
            continue;
        }

        printf("%s\n", line);

        process_command(vfs, line);
    }


    fclose(file_commands);
    printf(OK);
}

void format_command(VFS **vfs, int32_t vfs_size) {
    int id;
    inode *inodes;

    if (!vfs || !*vfs || vfs_size == 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "wb+");

    if (!(*vfs)->vfs_file) {
        printf(CANNOT_CREATE_FILE);
        return;
    }

    (*vfs)->is_formatted = true;

    (*vfs)->super_block = initialize_superblock(vfs_size);

    erase_vfs(vfs);

    rewind_vfs(vfs);

    (*vfs)->inode_bitmap = (int8_t *) calloc((DEFAULT_NUMBER_OF_INODES / 8), sizeof(int8_t));

    if (!(*vfs)->inode_bitmap) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    (*vfs)->data_bitmap = (int8_t *) calloc(((*vfs)->super_block->cluster_count / 8), sizeof(int8_t));

    if (!(*vfs)->data_bitmap) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    inodes = (inode *) calloc(DEFAULT_NUMBER_OF_INODES, sizeof(inode));

    if (!inodes) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    set_empty_bitmap((*vfs)->inode_bitmap);

    set_empty_bitmap((*vfs)->data_bitmap);

    for (int i = 0; i < DEFAULT_NUMBER_OF_INODES; i++) {
        inodes[i].node_id = ID_ITEM_FREE;
        inodes[i].type_of_file = NULL_TYPE;
        inodes[i].references = 0;
        inodes[i].file_size = 0;

        for (int j = 0; j < NUM_OF_DIRECT_LINK; ++j) {
            inodes[i].direct[j] = ID_ITEM_FREE;
        }

        inodes[i].indirect1 = ID_ITEM_FREE;
        inodes[i].indirect2 = ID_ITEM_FREE;
    }

    directory_item *root = new_directory_item(0, ".");

    (*vfs)->current = root;
    (*vfs)->root = root;

    inodes[0].node_id = 0;
    inodes[0].type_of_file = DIRECTORY_TYPE;
    inodes[0].references = 1;
    inodes[0].file_size = DIRECTORY_ITEM_SIZE;
    inodes[0].direct[0] = 0;

    id = find_free_inode(vfs);

    if (id == LACK_OF_FREE_ELEMENTS) {
        return;
    }

    change_value_of_bitmap_element((*vfs)->inode_bitmap, id);

    id = find_free_data_block(vfs);

    if (id == LACK_OF_FREE_ELEMENTS) {
        return;
    }

    change_value_of_bitmap_element((*vfs)->data_bitmap, id);

    directory_item *parent = new_directory_item(0, "..");

    inodes[0].file_size += DIRECTORY_ITEM_SIZE;

    write_vfs(vfs, ((*vfs)->super_block), sizeof(superblock), 1);

    seek_from_origin(vfs, (*vfs)->super_block->inode_bitmap_start_address);
    write_vfs(vfs, ((*vfs)->inode_bitmap), sizeof(int8_t), sizeof((*vfs)->inode_bitmap));

    seek_from_origin(vfs, (*vfs)->super_block->data_bitmap_start_address);
    write_vfs(vfs, ((*vfs)->data_bitmap), sizeof(int8_t), sizeof((*vfs)->data_bitmap));

    seek_from_origin(vfs, (*vfs)->super_block->inode_start_address);
    write_vfs(vfs, inodes, sizeof(inode), DEFAULT_NUMBER_OF_INODES);

    seek_from_origin(vfs, (*vfs)->super_block->data_start_address);
    write_vfs(vfs, root, sizeof(directory_item), 1);

    write_vfs(vfs, parent, sizeof(directory_item), 1);

    printf(OK);

    fclose((*vfs)->vfs_file);
}

void slink_command(VFS **vfs, char *src_path, char *dest_path) {
    int id, num_of_free_blocks;
    directory_item *src_directory, *dest_directory;
    char *src_file_name, *dest_file_name, *source_path, *destination_path, *path;

    if (!vfs || !*vfs || !src_path || !dest_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (str_empty(src_path) == true) {
        printf(FILE_NOT_FOUND);
        return;
    }

    if (str_empty(dest_path) == true) {
        printf(PATH_NOT_FOUND);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    if (strchr(src_path, '/') != NULL) {
        source_path = str_trim_and_split(src_path, &src_file_name, '/');

        if (!source_path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(src_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(src_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        src_directory = find_directory_item_by_path(vfs, source_path);

        if (!src_directory) {
            printf(FILE_NOT_FOUND);
            free(src_directory);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        src_file_name = src_path;

        if (str_equals(src_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            free(src_file_name);;
            return;
        } else if (str_equals(src_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            free(src_file_name);
            return;
        }

        src_directory = calloc(1, sizeof(directory_item));

        if (!src_directory) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(src_directory, (*vfs)->current, sizeof(directory_item));
    }

    if (strchr(dest_path, '/') != NULL) {
        destination_path = str_trim_and_split(dest_path, &dest_file_name, '/');

        if (!destination_path) {
            printf("The path was probably not entered with a correct slash. (/)\n");
            fclose((*vfs)->vfs_file);
            return;
        }

        if (str_equals(dest_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            free(src_directory);
            return;
        } else if (str_equals(dest_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            free(src_directory);
            return;
        }

        dest_directory = find_directory_item_by_path(vfs, destination_path);

        if (!dest_directory) {
            printf(PATH_NOT_FOUND);
            free(src_directory);
            free(dest_directory);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        dest_file_name = dest_path;

        if (str_equals(dest_file_name, ".") == true) {
            printf("File with this name cannot be moved or rename.\n");
            free(src_directory);
            fclose((*vfs)->vfs_file);
            return;
        } else if (str_equals(dest_file_name, "..") == true) {
            printf("File with this name cannot be moved or rename.\n");
            free(src_directory);
            fclose((*vfs)->vfs_file);
            return;
        }

        dest_directory = calloc(1, sizeof(directory_item));

        if (!dest_directory) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return;
        }

        memcpy(dest_directory, (*vfs)->current, sizeof(directory_item));
    }

    if (strlen(dest_file_name) > (FILENAME_LENGTH - 1)) {
        printf("File name too long.\n");
        free(src_directory);
        free(dest_directory);
        fclose((*vfs)->vfs_file);
        return;
    }


    inode *new_inode = (inode *) calloc(1, sizeof(inode));

    if (!new_inode) {
        free(new_inode);
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }


    directory_item *temp = find_directory_item_by_name(vfs, dest_directory, dest_file_name);

    if (temp) {
        printf(EXIST);
        free(src_directory);
        free(dest_directory);
        free(new_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    directory_item *src_dir_item = find_directory_item_by_name(vfs, src_directory, src_file_name);

    if (!src_dir_item) {
        printf(FILE_NOT_FOUND);
        free(src_dir_item);
        return;
    }

    inode *src_dir_inode = load_inode(vfs,src_dir_item->inode);

    if (src_dir_inode->type_of_file == DIRECTORY_TYPE){
        printf("Slink can only be created above the file.\n");
        free(src_directory);
        free(dest_directory);
        free(new_inode);
        free(temp);
        free(src_dir_item);
        return;
    }

    inode *src_inode = load_inode(vfs, src_directory->inode);

    inode *dest_inode = load_inode(vfs, dest_directory->inode);


    if ((dest_inode->file_size + DIRECTORY_ITEM_SIZE) > MAX_FILE_SIZE) {
        printf("Maximum file size exceeded.\n");
        free(src_directory);
        free(dest_directory);
        free(src_inode);
        free(dest_inode);
        free(new_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    num_of_free_blocks = num_free_data_blocks((*vfs)->data_bitmap, ((*vfs)->super_block->cluster_count / 8));

    if (num_of_free_blocks == LACK_OF_FREE_ELEMENTS) {
        printf("No free blocks.\n");
        free(src_directory);
        free(dest_directory);
        free(src_inode);
        free(dest_inode);
        free(new_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    if (num_of_free_blocks >= 2) {
        id = find_free_inode(vfs);

        if (id > 0) {
            change_value_of_bitmap_element((*vfs)->inode_bitmap, id);

            new_inode->node_id = id;
            new_inode->type_of_file = SLINK_TYPE;
            new_inode->references = 1;
            new_inode->file_size = CLUSTER_SIZE;

            id = find_free_data_block(vfs);

            if (id == LACK_OF_FREE_ELEMENTS) {
                printf("Lack of free blocks.\n");
                free(src_directory);
                free(dest_directory);
                free(src_inode);
                free(dest_inode);
                free(new_inode);
                free(temp);
                fclose((*vfs)->vfs_file);
                return;
            }

            new_inode->direct[0] = id;

            change_value_of_bitmap_element((*vfs)->data_bitmap, id);

            for (int i = 1; i < NUM_OF_DIRECT_LINK; ++i) {
                new_inode->direct[i] = ID_ITEM_FREE;
            }

            new_inode->indirect1 = ID_ITEM_FREE;
            new_inode->indirect2 = ID_ITEM_FREE;

            write_inode_to_vfs(vfs, new_inode);

            directory_item *dir_item = new_directory_item(new_inode->node_id, dest_file_name);

            write_directory_item(vfs, dest_directory, dir_item);

            path = get_path(vfs, src_directory, CLUSTER_SIZE);

            if (strlen(path) + strlen(src_file_name) > CLUSTER_SIZE) {
                printf("Too long path.");
                free(src_directory);
                free(dest_directory);
                free(src_inode);
                free(dest_inode);
                free(new_inode);
                free(path);
                free(temp);
                fclose((*vfs)->vfs_file);
                return;
            }

            strcat(path, "/");

            strcat(path, src_file_name);

            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (new_inode->direct[0] * CLUSTER_SIZE)));

            write_vfs(vfs, path, sizeof(char), CLUSTER_SIZE);

            write_bitmap_to_vfs(vfs, true);

            write_bitmap_to_vfs(vfs, false);

            free(dir_item);
        } else {
            printf("No free inodes.\n");
            free(src_directory);
            free(dest_directory);
            free(src_inode);
            free(dest_inode);
            free(new_inode);
            free(temp);
            fclose((*vfs)->vfs_file);
            return;
        }
    } else {
        printf("Lack of free blocks.\n");
        free(src_directory);
        free(dest_directory);
        free(src_inode);
        free(dest_inode);
        free(new_inode);
        free(temp);
        fclose((*vfs)->vfs_file);
        return;
    }

    free(src_directory);
    free(dest_directory);
    free(src_inode);
    free(dest_inode);
    free(new_inode);
    free(path);
    free(temp);

    printf(OK);

    fclose((*vfs)->vfs_file);
}

void debug_command(VFS **vfs) {
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->vfs_file = fopen((*vfs)->name, "rb+");

    if (!(*vfs)->vfs_file) {
        error_print(FAILED_TO_OPEN_FILE);
        return;
    }

    superblock sb = {};
    read_vfs(vfs, &sb, sizeof(superblock), 1);

    int8_t inode_bitmap[sb.data_bitmap_start_address - sb.inode_bitmap_start_address];
    int8_t data_bitmap[sb.inode_start_address - sb.data_bitmap_start_address];

    read_vfs(vfs, &inode_bitmap, sizeof(inode_bitmap), 1);

    read_vfs(vfs, &data_bitmap, sizeof(data_bitmap), 1);

    printf("\nsignature: %s\nvolume descriptor: %s\ndisk size: %d B\ncluster size: %d\ncluster count: %d\n"
           "inode_bitmap_start_address: %d\ndata_bitmap_start_address: %d\ninode_start_address: %d\ndata_start_address: %d\n",
           sb.signature, sb.volume_descriptor, sb.disk_size, sb.cluster_size, sb.cluster_count,
           sb.inode_bitmap_start_address,
           sb.data_bitmap_start_address, sb.inode_start_address, sb.data_start_address);

    printf("\nCurrent directory:\ninode: %d\ndir_name: %s\n", (*vfs)->current->inode, (*vfs)->current->item_name);

    print_bitmap(inode_bitmap, sizeof(inode_bitmap), "Inode bitmapa:");

    print_bitmap(data_bitmap, sizeof(data_bitmap), "Data bitmapa:");

    inode inodes[DEFAULT_NUMBER_OF_INODES] = {};

    read_vfs(vfs, &inodes, sizeof(inode), DEFAULT_NUMBER_OF_INODES);

    print_all_inodes(inodes);

    fclose((*vfs)->vfs_file);
}