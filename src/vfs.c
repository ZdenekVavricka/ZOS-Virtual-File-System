/**
 * @file vfs.c
 * @author Zdeněk Vavřička
 * @version 1.0
 */

#include "general_header.h"

superblock *initialize_superblock(int32_t vfs_size) {
    if (vfs_size == 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return NULL;
    }

    superblock *super_block = (superblock *) calloc(1, sizeof(superblock));

    if (!super_block) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    strcpy(super_block->signature, SUPER_BLOCK_SIGNATURE);
    strcpy(super_block->volume_descriptor, VOLUME_DESCRIPTOR);
    super_block->disk_size = vfs_size;
    super_block->cluster_size = CLUSTER_SIZE;

    super_block->inode_bitmap_start_address = sizeof(superblock);
    super_block->data_bitmap_start_address = super_block->inode_bitmap_start_address + (DEFAULT_NUMBER_OF_INODES / 8);
    super_block->inode_cluster_size = DEFAULT_NUMBER_OF_INODES * (int32_t) sizeof(inode);

    super_block->cluster_count = (vfs_size - (super_block->data_bitmap_start_address +
                                              super_block->inode_cluster_size)) / CLUSTER_SIZE;

    for (int i = 1; i < super_block->cluster_count; ++i) {
        if ((super_block->cluster_count / 8) < (i * CLUSTER_SIZE)) {
            super_block->cluster_count -= i;
            break;
        }
    }

    super_block->inode_start_address = (int32_t) ceil(
            super_block->data_bitmap_start_address + ((double) super_block->cluster_count / 8));

    super_block->data_start_address = super_block->inode_start_address + super_block->inode_cluster_size;

    return super_block;
}

void initialize_vfs(VFS **vfs, char *vfs_name) {
    if (!vfs_name) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs) = calloc(1, sizeof(VFS));

    if (!(*vfs)) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    (*vfs)->name = vfs_name;

    if (!file_exists(vfs_name)) {
        printf("VFS File not found, needs formatting.\n");
        (*vfs)->is_formatted = false;
    } else {
        FILE *temp_vfs_file = fopen(vfs_name, "rb+");

        if (!temp_vfs_file) {
            error_print(FAILED_TO_OPEN_FILE);
            return;
        }

        (*vfs)->is_formatted = true;
        (*vfs)->vfs_file = temp_vfs_file;

        load_vfs(vfs);

        fclose(temp_vfs_file);
    }
}

void load_vfs(VFS **vfs) {
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    (*vfs)->super_block = (superblock *) calloc(1, sizeof(superblock));

    if (!((*vfs)->super_block)) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    rewind_vfs(vfs);

    read_vfs(vfs, ((*vfs)->super_block), sizeof(superblock), 1);

    (*vfs)->inode_bitmap = (int8_t *) calloc((DEFAULT_NUMBER_OF_INODES / 8), sizeof(int8_t));

    if (!((*vfs)->inode_bitmap)) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    seek_from_origin(vfs, (*vfs)->super_block->inode_bitmap_start_address);
    read_vfs(vfs, ((*vfs)->inode_bitmap), sizeof(int8_t), (DEFAULT_NUMBER_OF_INODES / 8));

    (*vfs)->data_bitmap = (int8_t *) calloc(((*vfs)->super_block->cluster_count / 8), sizeof(int8_t));

    if (!((*vfs)->data_bitmap)) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    seek_from_origin(vfs, (*vfs)->super_block->data_bitmap_start_address);
    read_vfs(vfs, ((*vfs)->data_bitmap), sizeof(int8_t), ((*vfs)->super_block->cluster_count / 8));

    (*vfs)->current = (directory_item *) calloc(1, sizeof(directory_item));

    if (!((*vfs)->current)) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    (*vfs)->root = (directory_item *) calloc(1, sizeof(directory_item));

    if (!((*vfs)->root)) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    seek_from_origin(vfs, (*vfs)->super_block->data_start_address);
    read_vfs(vfs, (*vfs)->current, sizeof(directory_item), 1);

    seek_from_origin(vfs, (*vfs)->super_block->data_start_address);
    read_vfs(vfs, (*vfs)->root, sizeof(directory_item), 1);

    printf("VFS loaded successfully!\n");
}

void erase_vfs(VFS **vfs) {
    int multiplier = 1;
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (((*vfs)->super_block->disk_size) >= 1000000000) {
        multiplier = GB_MULTIPLIER;
    } else if (((*vfs)->super_block->disk_size) >= 300000000) {
        multiplier = MB_MULTIPLIER;
    }

    char *buffer = (char *) calloc((CLUSTER_SIZE * multiplier), sizeof(char));

    if (!buffer){
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    memset(buffer, 0, (CLUSTER_SIZE * multiplier));

    for (int i = 0; i < ((*vfs)->super_block->disk_size / (CLUSTER_SIZE * multiplier)); ++i) {
        write_vfs(vfs, buffer, sizeof(char), (CLUSTER_SIZE * multiplier));
    }

    free(buffer);
}

directory_item *new_directory_item(int32_t inode_id, char *name) {
    if (inode_id < 0 || !name) {
        return NULL;
    }

    char buff[FILENAME_LENGTH] = {'\0'};

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    strncpy(buff, name, strlen(name));
    dir_item->inode = inode_id;
    strncpy(dir_item->item_name, buff, FILENAME_LENGTH);

    return dir_item;
}

void set_empty_bitmap(int8_t *bitmap) {
    if (!bitmap) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    for (int i = 0; i < sizeof(bitmap); ++i) {
        bitmap[i] = 0b00000000;
    }
}

void change_value_of_bitmap_element(int8_t *bitmap, int id) {
    if (!bitmap || id < 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    int byte_block = id / 8;
    int bit_position = 7 - (id % 8);

    bitmap[byte_block] ^= 0b1 << bit_position;

}

void write_bitmap_to_vfs(VFS **vfs, bool bitmap_type) {
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    if (bitmap_type == true) {
        seek_from_origin(vfs, (*vfs)->super_block->inode_bitmap_start_address);
        write_vfs(vfs, (*vfs)->inode_bitmap, sizeof(int8_t), (DEFAULT_NUMBER_OF_INODES / 8));
    } else {
        seek_from_origin(vfs, (*vfs)->super_block->data_bitmap_start_address);
        write_vfs(vfs, (*vfs)->data_bitmap, sizeof(int8_t), ((*vfs)->super_block->cluster_count / 8));
    }
}

int32_t find_free_inode(VFS **vfs) {
    int32_t id_free_inode = LACK_OF_FREE_ELEMENTS;

    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return LACK_OF_FREE_ELEMENTS;
    }

    for (int i = 0; i < (DEFAULT_NUMBER_OF_INODES / 8); i++) {
        for (int j = 0; j < 8; ++j) {
            if (((*vfs)->inode_bitmap[i] & (0b10000000 >> j)) == 0) {
                id_free_inode = (i * 8) + j;
                return id_free_inode;
            }
        }
    }

    return id_free_inode;
}

int32_t find_free_data_block(VFS **vfs) {
    int32_t id_free_data_block = LACK_OF_FREE_ELEMENTS;

    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return LACK_OF_FREE_ELEMENTS;
    }

    for (int i = 0; i < ((*vfs)->super_block->cluster_count / 8); i++) {
        for (int j = 0; j < 8; ++j) {
            if (((*vfs)->data_bitmap[i] & (0b10000000 >> j)) == 0) {
                id_free_data_block = (i * 8) + j;
                return id_free_data_block;
            }
        }
    }

    return id_free_data_block;
}

int num_free_data_blocks(const int8_t *bitmap, int size) {
    int num_free_blocks = 0;


    if (!bitmap) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return LACK_OF_FREE_ELEMENTS;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 8; ++j) {
            if ((bitmap[i] & (0b10000000 >> j)) == 0) {
                num_free_blocks++;
            }
        }
    }

    return num_free_blocks;
}

void print_bitmap(const int8_t *bitmap, size_t size, const char *name_of_bitmap) {
    if (!bitmap || size < 0 || !name_of_bitmap) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    printf("\n%s\n", name_of_bitmap);

    for (int i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (bitmap[i] >> j) & 0b1);
        }
    }

    printf("\n");
}

void print_all_inodes(inode *inodes) {
    if (!inodes) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    printf("\nLoad inodes:\n");

    for (int i = 0; i < DEFAULT_NUMBER_OF_INODES; i++) {
        printf("id: %d, type_of_file: %d, references: %d, file_size: %d, direct1: %d, direct2: %d, direct3: %d, direct4: %d, direct5: %d, indirect1: %d, indirect2: %d\n",
               inodes[i].node_id, inodes[i].type_of_file, inodes[i].references, inodes[i].file_size,
               inodes[i].direct[0],
               inodes[i].direct[1], inodes[i].direct[2], inodes[i].direct[3], inodes[i].direct[4],
               inodes[i].indirect1,
               inodes[i].indirect2);
    }
}

directory_item *find_directory_item_by_path(VFS **vfs, char *path) {
    char *path_part;
    directory_item *dir;

    if (!vfs || !*vfs || !path) {
        return NULL;
    }

    dir = calloc(1, sizeof(directory_item));

    if (!dir) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    if (path[0] == '/') {
        memcpy(dir, (*vfs)->root, sizeof(directory_item));
    } else {
        memcpy(dir, (*vfs)->current, sizeof(directory_item));
    }

    path_part = strtok(path, PATH_DELIMETER);

    while (path_part != NULL) {
        if (str_equals(path_part, ".") == true) {
            path_part = strtok(NULL, PATH_DELIMETER);
            continue;
        } else if (str_equals(path_part, "..") == true) {
            dir = find_directory_item_by_name(vfs, dir, "..");

            if (!dir) {
                return NULL;
            }

            path_part = strtok(NULL, PATH_DELIMETER);

            continue;
        } else {
            dir = find_directory_item_by_name(vfs, dir, path_part);

            if (!dir) {
                return NULL;
            }

            path_part = strtok(NULL, PATH_DELIMETER);
        }
    }

    return dir;
}

directory_item *find_directory_item_by_name(VFS **vfs, directory_item *current, char *name) {
    inode *current_inode;
    int32_t *data_blocks;
    int block_count;

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }


    if (!vfs || !*vfs || !current) {
        return NULL;
    }

    current_inode = load_inode(vfs, current->inode);

    data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);


    for (int i = 0; i < block_count; ++i) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
        for (int j = 0; j < MAX_DIR_ITEMS_IN_BLOCK; ++j) {
            read_vfs(vfs, dir_item, sizeof(directory_item), 1);
            if (str_equals(dir_item->item_name, name) == true) {
                free(current_inode);
                return dir_item;
            }
        }
    }

    return NULL;
}

directory_item *find_directory_item_by_node_id(VFS **vfs, directory_item *current, int32_t node_id) {
    inode *current_inode;
    int32_t *data_blocks;
    int block_count;

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }


    if (!vfs || !*vfs || !current) {
        return NULL;
    }

    current_inode = load_inode(vfs, current->inode);

    data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);


    for (int i = 0; i < block_count; ++i) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
        for (int j = 0; j < MAX_DIR_ITEMS_IN_BLOCK; ++j) {
            read_vfs(vfs, dir_item, sizeof(directory_item), 1);
            if (dir_item->inode == node_id) {
                free(current_inode);
                return dir_item;
            }
        }
    }

    return NULL;
}

int print_content_of_directory(VFS **vfs, directory_item *current) {
    inode *current_inode;
    inode *temp_inode;
    int32_t *data_blocks;
    int block_count;

    if (!vfs || !*vfs || !current) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return FAILED_TO_ALLOCATE_MEMORY_ERROR;
    }

    current_inode = load_inode(vfs, current->inode);

    if (current_inode->type_of_file == DIRECTORY_TYPE) {
        data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);

        for (int i = 0; i < block_count; ++i) {
            for (int j = 0; j < (CLUSTER_SIZE / sizeof(directory_item)); ++j) {
                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE) +
                                       (j * (int) sizeof(directory_item))));
                read_vfs(vfs, dir_item, sizeof(directory_item), 1);
                if (dir_item->inode == 0 && (str_equals(dir_item->item_name, ".") == true ||
                                             str_equals(dir_item->item_name, "..") == true)) {
                    temp_inode = load_inode(vfs, dir_item->inode);
                    if (temp_inode->type_of_file == DIRECTORY_TYPE) {
                        printf("+ %s\n", dir_item->item_name);
                    } else {
                        printf("- %s\n", dir_item->item_name);
                    }
                    free(temp_inode);
                } else if (dir_item->inode != 0) {
                    temp_inode = load_inode(vfs, dir_item->inode);
                    if (temp_inode->type_of_file == DIRECTORY_TYPE) {
                        printf("+ %s\n", dir_item->item_name);
                    } else {
                        printf("- %s\n", dir_item->item_name);
                    }
                    free(temp_inode);
                }
            }
        }
    } else {
        free(dir_item);
        free(current_inode);
        return 1;
    }

    free(dir_item);
    free(current_inode);

    return 0;
}

int print_content_of_file(VFS **vfs, directory_item *current) {
    inode *current_inode;
    int32_t *data_blocks;
    int block_count;
    char *data;


    if (!vfs || !*vfs || !current) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    data = (char *) calloc(1, CLUSTER_SIZE);

    if (!data) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return FAILED_TO_ALLOCATE_MEMORY_ERROR;
    }

    current_inode = load_inode(vfs, current->inode);

    if (current_inode->type_of_file == FILE_TYPE) {
        data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);

        for (int i = 0; i < block_count; ++i) {
            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
            read_vfs(vfs, data, sizeof(char), CLUSTER_SIZE);
            printf("%s", data);
            memset(data, 0, CLUSTER_SIZE);
        }
    } else {
        free(data);
        free(current_inode);
        return 1;
    }

    printf("\n");

    free(data);
    free(current_inode);

    return 0;
}

void write_out_content_of_file(VFS **vfs, directory_item *current, FILE *output_file) {
    inode *current_inode;
    int32_t *data_blocks;
    int block_count, size;
    int8_t *data;


    if (!vfs || !*vfs || !current || !output_file) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    data = (int8_t *) calloc(1, CLUSTER_SIZE);

    if (!data) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    if (current_inode->type_of_file == FILE_TYPE) {
        data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);

        for (int i = 0; i < block_count; ++i) {
            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
            if (i == (block_count - 1)) {
                size = (current_inode->file_size - (i * CLUSTER_SIZE));
                read_vfs(vfs, data, sizeof(int8_t), size);
                fwrite(data, sizeof(int8_t), size, output_file);
            } else {
                read_vfs(vfs, data, sizeof(int8_t), CLUSTER_SIZE);
                fwrite(data, sizeof(int8_t), CLUSTER_SIZE, output_file);
            }
            memset(data, 0, CLUSTER_SIZE);
        }
    }

    free(data);
    free(current_inode);
}

inode *load_inode(VFS **vfs, int id) {
    if (!vfs || !*vfs || id < 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return NULL;
    }

    inode *load_inode_temp = (inode *) calloc(1, sizeof(inode));

    if (!load_inode_temp) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    seek_from_origin(vfs, ((*vfs)->super_block->inode_start_address + (id * ((int) sizeof(inode)))));

    read_vfs(vfs, load_inode_temp, sizeof(inode), 1);

    return load_inode_temp;
}

int32_t *get_id_of_data_blocks(VFS **vfs, int32_t node_id, int *block_count) {
    int32_t *data_blocks;
    int32_t data_block_id_indirect1, data_block_id_indirect2;
    int counter = 0;

    if (!vfs || !*vfs || node_id < 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return NULL;
    }

    int max_num_data_blocks = MAX_DATA_BLOCKS_FOR_INODE;

    inode *current_inode = load_inode(vfs, node_id);

    if (current_inode->type_of_file == DIRECTORY_TYPE) {
        data_blocks = (int32_t *) calloc(max_num_data_blocks, sizeof(int32_t));

        if (!data_blocks) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return NULL;
        }

        for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
            if (current_inode->direct[i] != ID_ITEM_FREE) {
                data_blocks[counter++] = current_inode->direct[i];
            }
        }

        if (current_inode->indirect1 != ID_ITEM_FREE) {
            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));
            for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
                read_vfs(vfs, &data_block_id_indirect1, sizeof(int32_t), 1);
                if (data_block_id_indirect1 > 0) {
                    data_blocks[counter++] = data_block_id_indirect1;
                }
            }
        }

        data_block_id_indirect1 = 0;
        data_block_id_indirect2 = 0;

        if (current_inode->indirect2 != ID_ITEM_FREE) {
            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));
            for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
                read_vfs(vfs, &data_block_id_indirect1, sizeof(int32_t), 1);
                if (data_block_id_indirect1 > 0) {
                    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address +
                                           (data_block_id_indirect1 * CLUSTER_SIZE)));
                    for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                        read_vfs(vfs, &data_block_id_indirect2, sizeof(int32_t), 1);
                        if (data_block_id_indirect2 > 0) {
                            data_blocks[counter++] = data_block_id_indirect2;
                        }
                    }
                    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address +
                                           (current_inode->indirect2 * CLUSTER_SIZE) +
                                           ((i + 1) * ((int) sizeof(int32_t)))));
                }
            }
        }

        *block_count = counter;

    } else if (current_inode->type_of_file == FILE_TYPE) {
        *block_count = current_inode->file_size / CLUSTER_SIZE;

        if (current_inode->file_size % CLUSTER_SIZE != 0) {
            (*block_count)++;
        }

        data_blocks = (int32_t *) calloc((*block_count), sizeof(int32_t));

        for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
            if (*block_count > counter) {
                data_blocks[counter++] = current_inode->direct[i];
            }
        }

        if (*block_count > 4 && current_inode->indirect1 != ID_ITEM_FREE) {
            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));
            for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
                if (*block_count >= counter) {
                    if (data_block_id_indirect1 > 0) {
                        read_vfs(vfs, &data_block_id_indirect1, sizeof(int32_t), 1);
                        data_blocks[counter++] = data_block_id_indirect1;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
        }

        if (*block_count > 4 && current_inode->indirect2 != ID_ITEM_FREE) {
            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));
            for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
                read_vfs(vfs, &data_block_id_indirect1, sizeof(int32_t), 1);
                if (data_block_id_indirect1 > 0) {
                    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address +
                                           (data_block_id_indirect1 * CLUSTER_SIZE)));
                    for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                        read_vfs(vfs, &data_block_id_indirect2, sizeof(int32_t), 1);
                        if (*block_count >= counter) {
                            if (data_block_id_indirect2 > 0) {
                                data_blocks[counter++] = data_block_id_indirect2;
                            }
                        } else {
                            break;
                        }
                    }
                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address +
                                      (current_inode->indirect2 * CLUSTER_SIZE) +
                                      ((i + 1) * ((int) sizeof(int32_t)))));
                }
            }
        }
    } else {
        data_blocks = (int32_t *) calloc(1, sizeof(int32_t));

        if (!data_blocks) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return NULL;
        }

        if (current_inode->direct[0] != ID_ITEM_FREE) {
            data_blocks[counter++] = current_inode->direct[0];
        }

        *block_count = counter;
    }

    free(current_inode);
    return data_blocks;
}

void write_inode_to_vfs(VFS **vfs, inode *write_inode) {
    if (!vfs || !*vfs || !write_inode) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    seek_from_origin(vfs, ((*vfs)->super_block->inode_start_address + (write_inode->node_id * ((int) sizeof(inode)))));

    write_vfs(vfs, write_inode, sizeof(inode), 1);
}

void set_inode_to_free(VFS **vfs, inode *current) {
    int id;

    if (!vfs || !*vfs || !current) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    id = current->node_id;

    current->node_id = ID_ITEM_FREE;
    current->type_of_file = NULL_TYPE;
    current->references = 0;
    current->file_size = 0;

    for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
        current->direct[i] = ID_ITEM_FREE;
    }

    current->indirect1 = ID_ITEM_FREE;
    current->indirect2 = ID_ITEM_FREE;

    seek_from_origin(vfs, ((*vfs)->super_block->inode_start_address + (id * ((int) sizeof(inode)))));

    write_vfs(vfs, current, sizeof(inode), 1);
}

void write_directory_item(VFS **vfs, directory_item *current, directory_item *write_dir_item) {
    int32_t id_free_block, temp_id1, temp_id2;
    int block_count;
    int32_t *data_blocks;
    inode *current_inode;

    if (!vfs || !*vfs || !write_dir_item) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);

    for (int i = 0; i < block_count; ++i) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
        for (int j = 0; j < MAX_DIR_ITEMS_IN_BLOCK; ++j) {
            read_vfs(vfs, dir_item, sizeof(directory_item), 1);
            if (dir_item->inode == 0 && str_equals(dir_item->item_name, "\0") == true) {
                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE) +
                                  (j * DIRECTORY_ITEM_SIZE)));
                write_vfs(vfs, write_dir_item, sizeof(directory_item), 1);
                current_inode->file_size += DIRECTORY_ITEM_SIZE;
                write_inode_to_vfs(vfs, current_inode);
                free(dir_item);
                free(current_inode);
                free(data_blocks);
                return;
            }
        }
    }

    id_free_block = find_free_data_block(vfs);

    if (id_free_block == LACK_OF_FREE_ELEMENTS) {
        return;
    }

    for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
        if (current_inode->direct[i] == ID_ITEM_FREE) {
            current_inode->direct[i] = id_free_block;
            change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
            write_bitmap_to_vfs(vfs, false);

            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (id_free_block * CLUSTER_SIZE)));
            write_vfs(vfs, write_dir_item, sizeof(directory_item), 1);
            current_inode->file_size += DIRECTORY_ITEM_SIZE;
            write_inode_to_vfs(vfs, current_inode);
            free(dir_item);
            free(current_inode);
            free(data_blocks);

            return;
        }
    }


    if (current_inode->indirect1 == ID_ITEM_FREE) {
        current_inode->indirect1 = id_free_block;
        change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
        write_bitmap_to_vfs(vfs, false);
        write_inode_to_vfs(vfs, current_inode);
    }

    if (current_inode->indirect1 != ID_ITEM_FREE) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));
        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 == 0) {
                id_free_block = find_free_data_block(vfs);

                if (id_free_block == LACK_OF_FREE_ELEMENTS) {
                    return;
                }

                change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
                write_bitmap_to_vfs(vfs, false);

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE) +
                                  (i * ((int) sizeof(int32_t)))));
                write_vfs(vfs, &id_free_block, sizeof(int32_t), 1);


                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (id_free_block * CLUSTER_SIZE)));
                write_vfs(vfs, write_dir_item, sizeof(directory_item), 1);

                current_inode->file_size += DIRECTORY_ITEM_SIZE;
                write_inode_to_vfs(vfs, current_inode);
                free(dir_item);
                free(current_inode);
                free(data_blocks);

                return;
            }
        }
    }

    temp_id1 = 0;

    if (current_inode->indirect2 == ID_ITEM_FREE) {
        current_inode->indirect2 = id_free_block;
        change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
        write_bitmap_to_vfs(vfs, false);
        write_inode_to_vfs(vfs, current_inode);
    }

    if (current_inode->indirect2 != ID_ITEM_FREE) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));
        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 == 0) {
                id_free_block = find_free_data_block(vfs);
                if (id_free_block == LACK_OF_FREE_ELEMENTS) {
                    return;
                }

                temp_id1 = id_free_block;

                change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
                write_bitmap_to_vfs(vfs, false);

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE) +
                                  (i * ((int) sizeof(int32_t)))));
                write_vfs(vfs, &id_free_block, sizeof(int32_t), 1);
            }

            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));

            for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                read_vfs(vfs, &temp_id2, sizeof(int32_t), 1);

                if (temp_id2 == 0) {
                    id_free_block = find_free_data_block(vfs);
                    if (id_free_block == LACK_OF_FREE_ELEMENTS) {
                        return;
                    }

                    change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
                    write_bitmap_to_vfs(vfs, false);

                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE) +
                                      (j * ((int) sizeof(int32_t)))));


                    write_vfs(vfs, &id_free_block, sizeof(int32_t), 1);

                    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (id_free_block * CLUSTER_SIZE)));

                    write_vfs(vfs, write_dir_item, sizeof(directory_item), 1);
                    current_inode->file_size += DIRECTORY_ITEM_SIZE;
                    write_inode_to_vfs(vfs, current_inode);
                    free(dir_item);
                    free(current_inode);
                    free(data_blocks);
                    return;
                }
            }
            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE) +
                                   ((i + 1) * ((int) sizeof(int32_t)))));
        }
    }
}

void rewrite_directory_item(VFS **vfs, directory_item *current, char *name, directory_item *rewrite_dir_item) {
    int i, j;
    bool found = false;
    inode *current_inode;
    int32_t *data_blocks;
    int block_count;

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    if (!vfs || !*vfs || !current || !rewrite_dir_item) {
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);

    for (i = 0; i < block_count; i++) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
        for (j = 0; j < MAX_DIR_ITEMS_IN_BLOCK; j++) {
            read_vfs(vfs, dir_item, sizeof(directory_item), 1);
            if (str_equals(dir_item->item_name, name) == true) {
                found = true;
                break;
            }
        }
        if (found == true) {
            break;
        }
    }

    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE) +
                           (j * DIRECTORY_ITEM_SIZE)));

    write_vfs(vfs, rewrite_dir_item, sizeof(directory_item), 1);

    free(dir_item);
}

void delete_dir_item(VFS **vfs, directory_item *current, directory_item *delete_dir_item) {
    int i, j;
    bool found = false;
    int8_t delete_buffer[sizeof(directory_item)] = {0};
    inode *current_inode;
    int32_t *data_blocks;
    int block_count;

    directory_item *dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    if (!vfs || !*vfs || !current || !delete_dir_item) {
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    data_blocks = get_id_of_data_blocks(vfs, current_inode->node_id, &block_count);

    for (i = 0; i < block_count; i++) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
        for (j = 0; j < MAX_DIR_ITEMS_IN_BLOCK; j++) {
            read_vfs(vfs, dir_item, sizeof(directory_item), 1);
            if (str_equals(dir_item->item_name, delete_dir_item->item_name) == true) {
                found = true;
                break;
            }
        }
        if (found == true) {
            break;
        }
    }

    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE) +
                           (j * DIRECTORY_ITEM_SIZE)));

    write_vfs(vfs, delete_buffer, sizeof(int8_t), sizeof(directory_item));

    free(dir_item);
}

void free_all_file_data_blocks(VFS **vfs, directory_item *current) {
    int32_t delete_number = 0;
    int32_t temp_id1, temp_id2;
    int8_t delete_buffer[CLUSTER_SIZE] = {0};
    inode *current_inode;

    if (!vfs || !*vfs || !current) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
        if (current_inode->direct[i] != ID_ITEM_FREE) {
            change_value_of_bitmap_element((*vfs)->data_bitmap, current_inode->direct[i]);

            write_bitmap_to_vfs(vfs, false);

            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (current_inode->direct[i] * CLUSTER_SIZE)));

            write_vfs(vfs, delete_buffer, CLUSTER_SIZE, 1);
        }
    }


    if (current_inode->indirect1 != ID_ITEM_FREE) {
        change_value_of_bitmap_element((*vfs)->data_bitmap, current_inode->indirect1);
        write_bitmap_to_vfs(vfs, false);

        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));
        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));

                write_vfs(vfs, delete_buffer, CLUSTER_SIZE, 1);

                change_value_of_bitmap_element((*vfs)->data_bitmap, temp_id1);

                write_bitmap_to_vfs(vfs, false);

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE) +
                                  (i * ((int) sizeof(int32_t)))));

                write_vfs(vfs, &delete_number, sizeof(int32_t), 1);
            }
        }
    }

    temp_id1 = 0;

    if (current_inode->indirect2 != ID_ITEM_FREE) {
        change_value_of_bitmap_element((*vfs)->data_bitmap, current_inode->indirect2);
        write_bitmap_to_vfs(vfs, false);

        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));
                for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                    read_vfs(vfs, &temp_id2, sizeof(int32_t), 1);
                    if (temp_id2 != 0) {
                        seek_from_origin(vfs,
                                         ((*vfs)->super_block->data_start_address + (temp_id2 * CLUSTER_SIZE)));

                        write_vfs(vfs, delete_buffer, CLUSTER_SIZE, 1);

                        change_value_of_bitmap_element((*vfs)->data_bitmap, temp_id2);

                        write_bitmap_to_vfs(vfs, false);

                        seek_from_origin(vfs,
                                         ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE) +
                                          ((j + 1) * ((int) sizeof(int32_t)))));
                    }
                }
            }
        }

        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));

                write_vfs(vfs, delete_buffer, CLUSTER_SIZE, 1);

                change_value_of_bitmap_element((*vfs)->data_bitmap, temp_id1);

                write_bitmap_to_vfs(vfs, false);

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE) +
                                  (i * ((int) sizeof(int32_t)))));

                write_vfs(vfs, &delete_number, sizeof(int32_t), 1);
            }
        }
    }
}

void write_file(VFS **vfs, directory_item *current, int8_t *data) {
    int32_t id_free_block, temp_id1, temp_id2;
    inode *current_inode;

    if (!vfs || !*vfs || !current || !data) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    id_free_block = find_free_data_block(vfs);

    if (id_free_block == LACK_OF_FREE_ELEMENTS) {
        return;
    }

    for (int i = 0; i < NUM_OF_DIRECT_LINK; ++i) {
        if (current_inode->direct[i] == ID_ITEM_FREE) {
            current_inode->direct[i] = id_free_block;
            change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);

            write_bitmap_to_vfs(vfs, false);
            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (id_free_block * CLUSTER_SIZE)));

            write_vfs(vfs, data, CLUSTER_SIZE, 1);
            write_inode_to_vfs(vfs, current_inode);
            free(current_inode);
            return;
        }
    }


    if (current_inode->indirect1 == ID_ITEM_FREE) {
        current_inode->indirect1 = id_free_block;
        change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
        write_bitmap_to_vfs(vfs, false);
        write_inode_to_vfs(vfs, current_inode);
    }

    if (current_inode->indirect1 != ID_ITEM_FREE) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));
        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 == 0) {
                id_free_block = find_free_data_block(vfs);

                if (id_free_block == LACK_OF_FREE_ELEMENTS) {
                    return;
                }

                change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
                write_bitmap_to_vfs(vfs, false);

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE) +
                                  (i * ((int) sizeof(int32_t)))));
                write_vfs(vfs, &id_free_block, sizeof(int32_t), 1);


                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (id_free_block * CLUSTER_SIZE)));
                write_vfs(vfs, data, CLUSTER_SIZE, 1);
                write_inode_to_vfs(vfs, current_inode);
                free(current_inode);
                return;
            }
        }
    }

    temp_id1 = 0;

    if (current_inode->indirect2 == ID_ITEM_FREE) {
        current_inode->indirect2 = id_free_block;
        change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
        write_bitmap_to_vfs(vfs, false);
        write_inode_to_vfs(vfs, current_inode);
    }

    if (current_inode->indirect2 != ID_ITEM_FREE) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));
        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 == 0) {
                id_free_block = find_free_data_block(vfs);
                if (id_free_block == LACK_OF_FREE_ELEMENTS) {
                    return;
                }

                temp_id1 = id_free_block;

                change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
                write_bitmap_to_vfs(vfs, false);

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE) +
                                  (i * ((int) sizeof(int32_t)))));
                write_vfs(vfs, &id_free_block, sizeof(int32_t), 1);
            }

            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));

            for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                read_vfs(vfs, &temp_id2, sizeof(int32_t), 1);

                if (temp_id2 == 0) {
                    id_free_block = find_free_data_block(vfs);
                    if (id_free_block == LACK_OF_FREE_ELEMENTS) {
                        return;
                    }

                    change_value_of_bitmap_element((*vfs)->data_bitmap, id_free_block);
                    write_bitmap_to_vfs(vfs, false);

                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE) +
                                      (j * ((int) sizeof(int32_t)))));


                    write_vfs(vfs, &id_free_block, sizeof(int32_t), 1);

                    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (id_free_block * CLUSTER_SIZE)));

                    write_vfs(vfs, data, CLUSTER_SIZE, 1);
                    write_inode_to_vfs(vfs, current_inode);

                    free(current_inode);
                    return;
                }
            }
            seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE) +
                                   ((i + 1) * ((int) sizeof(int32_t)))));
        }
    }
}

void copy_file_data_blocks(VFS **vfs, directory_item *src_dir_item, directory_item *dest_dir_item) {
    int8_t *data;
    int32_t *data_blocks;
    int block_count;

    if (!vfs || !*vfs || !src_dir_item || !dest_dir_item) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    data = (int8_t *) calloc(CLUSTER_SIZE, sizeof(int8_t));

    if (!data) {
        fclose((*vfs)->vfs_file);
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    inode *src_inode = load_inode(vfs, src_dir_item->inode);

    inode *dest_inode = load_inode(vfs, dest_dir_item->inode);

    data_blocks = get_id_of_data_blocks(vfs, src_inode->node_id, &block_count);

    for (int i = 0; i < block_count; ++i) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (data_blocks[i] * CLUSTER_SIZE)));
        read_vfs(vfs, data, sizeof(int8_t), CLUSTER_SIZE);
        write_file(vfs, dest_dir_item, data);
        memset(data, 0, CLUSTER_SIZE);
    }

    free(data);
}

void return_data_blocks(VFS **vfs, directory_item *current) {
    int32_t temp_id1, temp_id2;
    int32_t delete_number = 0;
    inode *current_inode;
    directory_item *temp_dir_item;
    bool found = false;

    if (!vfs || !*vfs || !current) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    temp_dir_item = (directory_item *) calloc(1, sizeof(directory_item));

    if (!temp_dir_item) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return;
    }

    current_inode = load_inode(vfs, current->inode);

    if (current_inode->indirect2 != ID_ITEM_FREE) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));
                for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                    read_vfs(vfs, &temp_id2, sizeof(int32_t), 1);
                    if (temp_id2 != 0) {
                        seek_from_origin(vfs,
                                         ((*vfs)->super_block->data_start_address + (temp_id2 * CLUSTER_SIZE)));
                        for (int k = 0; k < (CLUSTER_SIZE / sizeof(int32_t)); ++k) {

                            read_vfs(vfs, temp_dir_item, sizeof(directory_item), 1);
                            if (temp_dir_item->inode != 0 && str_equals(temp_dir_item->item_name, "0") == false) {
                                found = true;
                                break;
                            }
                        }

                        if (found == false) {
                            change_value_of_bitmap_element((*vfs)->data_bitmap, temp_id2);
                            write_bitmap_to_vfs(vfs, false);

                            seek_from_origin(vfs,
                                             ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE) +
                                              (j * ((int) sizeof(int32_t)))));

                            write_vfs(vfs, &delete_number, sizeof(int32_t), 1);
                        } else {
                            found = false;
                            seek_from_origin(vfs,
                                             ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE) +
                                              ((j + 1) * ((int) sizeof(int32_t)))));

                        }
                    }
                }

                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE) +
                                  ((i + 1) * ((int) sizeof(int32_t)))));
            }
        }

        found = false;

        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                seek_from_origin(vfs,
                                 ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));
                for (int j = 0; j < (CLUSTER_SIZE / sizeof(int32_t)); ++j) {
                    read_vfs(vfs, &temp_id2, sizeof(int32_t), 1);
                    if (temp_id2 != 0) {
                        found = true;
                        break;
                    }
                }

                if (found == false) {
                    change_value_of_bitmap_element((*vfs)->data_bitmap, temp_id1);
                    write_bitmap_to_vfs(vfs, false);

                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address +
                                      (current_inode->indirect2 * CLUSTER_SIZE) +
                                      (i * ((int) sizeof(int32_t)))));

                    write_vfs(vfs, &delete_number, sizeof(int32_t), 1);
                } else {
                    found = false;
                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address +
                                      (current_inode->indirect2 * CLUSTER_SIZE) +
                                      ((i + 1) * ((int) sizeof(int32_t)))));
                }
            }
        }

        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect2 * CLUSTER_SIZE)));

        found = false;

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                found = true;
                break;
            }
        }

        if (found == false) {

            change_value_of_bitmap_element((*vfs)->data_bitmap, current_inode->indirect2);

            write_bitmap_to_vfs(vfs, false);

            current_inode->indirect2 = ID_ITEM_FREE;

            write_inode_to_vfs(vfs, current_inode);
        } else {
            found = false;
        }
    }

    found = false;

    if (current_inode->indirect1 != ID_ITEM_FREE) {
        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (temp_id1 * CLUSTER_SIZE)));

                for (int j = 0; j < (CLUSTER_SIZE / DIRECTORY_ITEM_SIZE); ++j) {
                    read_vfs(vfs, temp_dir_item, sizeof(directory_item), 1);
                    if (temp_dir_item->inode != 0 && str_equals(temp_dir_item->item_name, "0") == false) {
                        found = true;
                        break;
                    }
                }

                if (found == false) {
                    change_value_of_bitmap_element((*vfs)->data_bitmap, temp_id1);

                    write_bitmap_to_vfs(vfs, false);

                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address +
                                      (current_inode->indirect1 * CLUSTER_SIZE) +
                                      (i * ((int) sizeof(int32_t)))));
                    write_vfs(vfs, &delete_number, sizeof(int32_t), 1);
                } else {
                    found = false;

                    seek_from_origin(vfs,
                                     ((*vfs)->super_block->data_start_address +
                                      (current_inode->indirect1 * CLUSTER_SIZE) +
                                      ((i + 1) * ((int) sizeof(int32_t)))));
                }
            }
        }

        seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (current_inode->indirect1 * CLUSTER_SIZE)));

        found = false;

        for (int i = 0; i < (CLUSTER_SIZE / sizeof(int32_t)); ++i) {
            read_vfs(vfs, &temp_id1, sizeof(int32_t), 1);
            if (temp_id1 != 0) {
                found = true;
                break;
            }
        }

        if (found == false) {
            found = false;

            change_value_of_bitmap_element((*vfs)->data_bitmap, current_inode->indirect1);

            write_bitmap_to_vfs(vfs, false);

            current_inode->indirect1 = ID_ITEM_FREE;

            write_inode_to_vfs(vfs, current_inode);
        }
    }


    for (int i = (NUM_OF_DIRECT_LINK - 1); i > 0; --i) {
        if (current_inode->direct[i] != ID_ITEM_FREE) {
            seek_from_origin(vfs,
                             ((*vfs)->super_block->data_start_address + (current_inode->direct[i] * CLUSTER_SIZE)));
            for (int j = 0; j < (CLUSTER_SIZE / DIRECTORY_ITEM_SIZE); ++j) {
                read_vfs(vfs, temp_dir_item, sizeof(directory_item), 1);
                if (temp_dir_item->inode != 0 && str_equals(temp_dir_item->item_name, "0") == false) {
                    found = true;
                    break;
                }
            }

            if (found == false) {
                change_value_of_bitmap_element((*vfs)->data_bitmap, current_inode->direct[i]);

                write_bitmap_to_vfs(vfs, false);

                current_inode->direct[i] = ID_ITEM_FREE;

                write_inode_to_vfs(vfs, current_inode);
            } else {
                found = false;
            }
        }
    }
}

char *get_path(VFS **vfs, directory_item *current_directory, int size_of_path) {
    char temp_path[50][LINE_LENGTH] = {0};
    directory_item *current;
    directory_item *parent;
    directory_item *temp;
    int count = 0;

    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return NULL;
    }

    char *path = (char *) calloc(size_of_path, sizeof(char));

    if (!path) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    current = calloc(1, sizeof(directory_item));

    if (!current) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    memcpy(current, current_directory, sizeof(directory_item));

    while (true) {
        parent = find_directory_item_by_name(vfs, current, "..");

        temp = find_directory_item_by_node_id(vfs, parent, current->inode);

        if (temp->inode == 0) {
            break;
        }


        strcpy(temp_path[count++], temp->item_name);

        current = parent;
    }

    path[0] = '/';

    for (int i = (count - 1); i >= 0; --i) {
        strcat(path, temp_path[i]);
        if (i != 0) {
            strcat(path, "/");
        }
    }

    free(parent);
    free(temp);

    return path;
}

int seek_from_origin(VFS **vfs, long int offset) {
    if (!vfs || !*vfs || offset < 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    return fseek((*vfs)->vfs_file, offset, SEEK_SET);
}

bool test_slink(VFS **vfs, char *string_path) {
    char *path, *file_name;
    directory_item *directory, *file_dir_item;

    if (!vfs || !*vfs || !string_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return false;
    }

    if (strchr(string_path, '/') != NULL) {
        path = str_trim_and_split(string_path, &file_name, '/');

        if (!path) {
            return false;
        }

        if (str_equals(file_name, ".") == true) {
            return false;
        } else if (str_equals(file_name, "..") == true) {
            return false;
        }

        directory = find_directory_item_by_path(vfs, path);

        if (!directory) {
            return false;
        }

    } else {
        file_name = string_path;

        if (str_equals(file_name, ".") == true) {
            return false;
        } else if (str_equals(file_name, "..") == true) {
            return false;
        }

        directory = calloc(1, sizeof(directory_item));

        if (!directory) {
            error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
            return false;
        }

        memcpy(directory, (*vfs)->current, sizeof(directory_item));
    }

    if (strlen(file_name) > (FILENAME_LENGTH - 1)) {
        return false;
    }

    file_dir_item = find_directory_item_by_name(vfs, directory, file_name);

    if (!file_dir_item) {
        free(directory);
        return false;
    }

    inode *file_inode = load_inode(vfs, file_dir_item->inode);

    if (file_inode->type_of_file != SLINK_TYPE) {
        free(directory);
        free(file_dir_item);
        free(file_inode);
        return false;
    }

    free(directory);
    free(file_dir_item);
    free(file_inode);

    return true;
}

char *get_path_from_slink(VFS **vfs, char *string_path) {

    if (!vfs || !*vfs || !string_path) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return NULL;
    }

    directory_item *slink_dir_item = find_directory_item_by_path(vfs, string_path);

    if (!slink_dir_item) {
        return NULL;
    }

    inode *slink_inode = load_inode(vfs, slink_dir_item->inode);

    char *path = (char *) calloc(CLUSTER_SIZE, sizeof(char));

    if (!path) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    char *temp_path = (char *) calloc(CLUSTER_SIZE, sizeof(char));

    if (!temp_path) {
        error_print(FAILED_TO_ALLOCATE_MEMORY_ERROR);
        return NULL;
    }

    seek_from_origin(vfs, ((*vfs)->super_block->data_start_address + (slink_inode->direct[0] * CLUSTER_SIZE)));

    read_vfs(vfs, path, sizeof(char), CLUSTER_SIZE);

    if (str_empty(path) == true) {
        free(slink_dir_item);
        free(slink_inode);
        free(path);
        return NULL;
    }

    memcpy(temp_path, path, CLUSTER_SIZE);

    directory_item *temp = find_directory_item_by_path(vfs, temp_path);

    if (!temp) {
        return NULL;
    }

    free(slink_dir_item);
    free(slink_inode);
    free(temp_path);

    return path;
}

void rewind_vfs(VFS **vfs) {
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    rewind((*vfs)->vfs_file);
}

size_t write_vfs(VFS **vfs, const void *ptr, size_t size, size_t count) {
    if (!vfs || !*vfs || size < 0 || count < 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    size_t temp = fwrite(ptr, size, count, (*vfs)->vfs_file);

    flush_vfs(vfs);

    return temp;
}

size_t read_vfs(VFS **vfs, void *ptr, size_t size, size_t count) {
    if (!vfs || !*vfs || size < 0 || count < 0) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return FAILED_TO_PASS_PARAMETERS_ERROR;
    }

    return fread(ptr, size, count, (*vfs)->vfs_file);
}

void flush_vfs(VFS **vfs) {
    if (!vfs || !*vfs) {
        error_print(FAILED_TO_PASS_PARAMETERS_ERROR);
        return;
    }

    fflush((*vfs)->vfs_file);
}