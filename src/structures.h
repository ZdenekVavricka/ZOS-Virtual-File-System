/**
 * @file structures.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s definovanými strukturami.
 * @version 1.0
 */

#ifndef FILE_SYSTEM_STRUCTURES_H
#define FILE_SYSTEM_STRUCTURES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "constants.h"

/**
 * @brief Definice struktury directory_item
 */
typedef struct directory_item {
    int32_t inode;                      /*id inode*/
    char item_name[FILENAME_LENGTH];    /*jméno souboru*/
}directory_item;

/**
 * @brief Definice struktury inode
 */
typedef struct inode {
    int32_t node_id;        /*id inode*/
    int16_t type_of_file;   /*typ souboru*/
    int16_t references;     /*počet referencí*/
    int32_t file_size;      /*velikost souboru*/
    int32_t direct[5];      /*pole přímých odkazů*/
    int32_t indirect1;      /*nepřímý odkaz prvního řádu*/
    int32_t indirect2;      /*nepřímý odkaz druhého řádu*/
} inode;

/**
 * @brief Definice struktury superblock
 */
typedef struct superblock {
    char signature[SIGNATURE_LENGTH];                   /*jméno souborového systému*/
    char volume_descriptor[VOLUME_DESCRIPTOR_LENGTH];   /*popis souborového systému*/
    int32_t disk_size;                                  /*velikost souborového systému*/
    int32_t cluster_size;                               /*velikost datového bloku*/
    int32_t cluster_count;                              /*počet datových bloků*/
    int32_t inode_cluster_size;                         /*počet i-nodů v souborovém systému*/
    int32_t inode_bitmap_start_address;                 /*počáteční adresa bitmapy i-nodů*/
    int32_t inode_start_address;                        /*počáteční adresa i-nodů*/
    int32_t data_bitmap_start_address;                  /*počáteční adresa bitmapy datových bloků*/
    int32_t data_start_address;                         /*počáteční adresa datových bloků*/
} superblock;

/**
 * @brief Definice struktury vfs
 */
typedef struct vfs {
    superblock *super_block;    /*struktura superblock*/
    int8_t *inode_bitmap;       /*bitmapa i-nodů*/
    int8_t *data_bitmap;        /*bitmapy datových bloků*/

    directory_item *root;       /*directory_item kořenového adresáře*/
    directory_item *current;    /*directory_item aktuálního adresáře*/

    bool is_formatted;          /*proměnná držící stav zdali je souborový systém naformátován*/

    char *name;                 /*jméno souborového systému*/
    FILE *vfs_file;             /*ukazatel na soubor souborového systému*/
} VFS;

#endif
