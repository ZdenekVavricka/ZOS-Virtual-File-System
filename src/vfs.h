/**
 * @file vfs.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s funkcemi určenými pro práci se souborovým systémem.
 * @version 1.0
 */

#ifndef FILE_SYSTEM_VFS_H
#define FILE_SYSTEM_VFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structures.h"

/**
 * @brief Funkce pro inicializaci superbloku
 * @param vfs_size velikost souborového systému
 * @return super_block inicializovaný superblok
 */
superblock *initialize_superblock(int32_t vfs_size);

/**
 * @brief Funkce pro inicializaci struktury souborového systému
 * @param vfs struktura souborového systému
 * @param vfs_name jméno souborového systému
 */
void initialize_vfs(VFS **vfs, char *vfs_name) ;

/**
 * @brief Funkce pro načtení již vytvořeného souborového systému
 * @param vfs struktura souborového systému
 */
void load_vfs(VFS **vfs);

/**
 * @brief Funkce pro vymazání/zapsání do souborového systému a nasteví všech bytů na 0
 * @param vfs struktura souborového systému
 */
void erase_vfs(VFS **vfs);

/**
 * @brief Funkce pro vytvroření nového dir_itemu
 * @param inode_id id inodu přiřazeného novému souboru
 * @param name jméno souboru
 * @return new_dir_item nový dir_item
 */
directory_item *new_directory_item(int32_t inode_id, char *name);

/**
 * @brief Funkce pro nastavení prázdné bitmapy
 * @param bitmap bitmapa určená pro nastavení
 */
void set_empty_bitmap(int8_t *bitmap);

/**
 * @brief Funkce pro změnu hodnoty v bitmapě,
 * @param bitmap, kde má být provedena změna
 * @param id pozice, která má být změněna
 */
void change_value_of_bitmap_element(int8_t *bitmap, int id);

/**
 * @brief Funkce pro zápis bitmapy do souborového systému
 * @param vfs struktura souborového systému
 * @param bitmap_type true - bitmapa inodů
 *                    false - bitmapa datových bloků
 */
void write_bitmap_to_vfs(VFS **vfs, bool bitmap_type);

/**
 * @brief Funkce pro nalezení volného i-nodu
 * @param vfs struktura souborového systému
 * @return id volného inodu
 */
int32_t find_free_inode(VFS **vfs);

/**
 * @brief Funkce pro nalezení volného datového bloku
 * @param vfs struktura souborového systému
 * @return id volného datového bloku
 */
int32_t find_free_data_block(VFS **vfs);

/**
 * @brief Funkce pro zjištění počtu volných bloků
 * @param data_bitmap datová bitmapa
 * @param size velikost datové bitmapy
 * @return počet volných datových bloků
 */
int num_free_data_blocks(const int8_t *data_bitmap, int size);

/**
 * @brief Funkce pro výpis bitmapy.
 * @param bitmap bitmapa, která má být vypsána
 * @param size velikost bitmapy
 * @param name_of_bitmap jméno bitmapy
 */
void print_bitmap(const int8_t *bitmap, size_t size, const char *name_of_bitmap);

/**
 * @brief Funkce pro vypsání všech i-nodů
 * @param inodes pole i-nodů
 */
void print_all_inodes(inode *inodes);

/**
 * @brief Funkce pro nalezení directory_item podle zadané cesty
 * @param vfs struktura souborového systému
 * @param path cesta k dir_itemu
 * @return nalezený dir_item, jinak NULL
 */
directory_item *find_directory_item_by_path(VFS **vfs, char *path);

/**
 * @brief Funkce pro nalezení directory_item podle názvu
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @param name jméno dir_itemu
 * @return dir_item, jinak NULL
 */
directory_item *find_directory_item_by_name(VFS **vfs, directory_item *current, char *name);

/**
 * @brief Funkce pro nalezení directory_item podle i-node id
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @param node_id i-node id
 * @return dir_item, jinak NULL
 */
directory_item *find_directory_item_by_node_id(VFS **vfs, directory_item *current, int32_t node_id);

/**
 * @brief Funkce pro výpis obsahu adresáře
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @return 0 pokud se vše podařilo
 *         1 pokud se nejednalo o adresář
 */
int print_content_of_directory(VFS **vfs, directory_item *current);

/**
 * @brief Funkce pro výpis obsahu souboru
 * @param vfs struktura souborového systému
 * @param current aktuální soubor
 * @return 0 pokud se vše podařilo
 *         1 pokud se nejednalo o soubor
 */
int print_content_of_file(VFS **vfs, directory_item *current);

/**
 * @brief Funkce pro výpis obsahu souboru do souboru na pevném disku
 * @param vfs struktura souborového systému
 * @param current aktuální soubor
 * @param output_file výstupní soubor
 */
void write_out_content_of_file(VFS **vfs, directory_item *current, FILE *output_file);

/**
 * @brief Funkce pro načtení i-node
 * @param vfs struktura souborového systému
 * @param id i-node
 * @return i-node
 */
inode *load_inode(VFS **vfs, int id);

/**
 * @brief Funkce pro zjištěni id obsazených datových bloků daným soubore/adresářem
 * @param vfs struktura souborového systému
 * @param node_id i-node id
 * @param block_count ukazatel na počet obsazených bloků
 * @return pole id obsazených datových bloků
 */
int32_t *get_id_of_data_blocks(VFS **vfs, int32_t node_id, int *block_count);

/**
 * @brief Funkce pro zápis i-node do souborového systému
 * @param vfs struktura souborového systému
 * @param write_inode i-node, který má být zapsán
 */
void write_inode_to_vfs(VFS **vfs, inode *write_inode);

/**
 * @brief Funkce pro nastavení i-node na volný i-node
 * @param vfs struktura souborového systému
 * @param current i-node, který má být nastaven na volný
 */
void set_inode_to_free(VFS **vfs, inode *current);

/**
 * @brief Funkce pro zapsání dir_itemu do souborového systému
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @param write_dir_item dir_item, který má být zapsán
 */
void write_directory_item(VFS **vfs, directory_item *current, directory_item *write_dir_item);

/**
 * @brief Funkce pro přepsání dir_itemu do souborového systému
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @param name jméno souboru v dir_itemu, který má být přepsán
 * @param rewrite_dir_item nový dir_item, který má být zapsán
 */
void rewrite_directory_item(VFS **vfs, directory_item *current, char *name, directory_item *rewrite_dir_item);

/**
 * @brief Funkce pro smazání dir_itemu
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @param delete_dir_item dir_item, který má být smazán
 */
void delete_dir_item(VFS **vfs, directory_item *current, directory_item *delete_dir_item);

/**
 * @brief Funkce pro uvolnění všech datových bloků souboru
 * @param vfs struktura souborového systému
 * @param current aktuální soubor
 */
void free_all_file_data_blocks(VFS **vfs, directory_item *current);

/**
 * @brief Funkce pro zapsání/vytvoření souboru v souborovém systému.
 * @param vfs struktura souborového systému
 * @param current aktuální adresář
 * @param data data, která májí být zapsána
 */
void write_file(VFS **vfs, directory_item *current, int8_t *data);

/**
 * @brief Funkce pro kopírování datových bloků souboru.
 * @param vfs struktura souborového systému
 * @param src_dir_item dir_item zdroje
 * @param dest_dir_item dir_item cíle
 */
void copy_file_data_blocks(VFS **vfs, directory_item *src_dir_item, directory_item *dest_dir_item);

/**
 * @brief Funkce pro navrácení volných datových bloků souboru
 * @param vfs struktura souborového systému
 * @param current aktuální soubor
 */
void return_data_blocks(VFS **vfs, directory_item *current);

/**
 * @brief Funkce pro zjištění absolutní cesty do aktuálního adresáře nebo k aktuálnímu souboru.
 * @param vfs struktura souborového systému
 * @param current_directory aktuální adresář
 * @param size_of_path velikost bufferu pro uložení cesty
 * @return path absolutní cesta
 */
char *get_path(VFS **vfs, directory_item *current_directory, int size_of_path);

/**
 * @brief Funkce pro nastavení pozice od začátku souborového systému.
 * @param vfs struktura souborového systému
 * @param offset o kolik se musím posunout v souboru
 * @return pozice v souboru
 */
int seek_from_origin(VFS **vfs, long int offset);

/**
 * @brief Funkce pro otestování zdali se nejedná o symbolický link
 * @param vfs struktura souborového systému
 * @param string_path cesta k souboru
 * @return true pokud se jedná o symbolický link
 *         false pokud se nejedná o slink
 */
bool test_slink(VFS **vfs, char *string_path);

/**
 * @brief Funkce pro získání absolutní cesty uložené v symbolickém linku
 * @param vfs struktura souborového systému
 * @param string_path cesta k souboru symbolického linku
 * @return path absolutní cesta
 */
char *get_path_from_slink(VFS **vfs, char *string_path);

/**
 * @brief Funkce pro navrácení se na počáteční pozici v souboru souborového systému.
 * @param vfs struktura souborového systému
 */
void rewind_vfs(VFS **vfs);

/**
 * @brief Funkce pro zápis do souborového systému
 * @param vfs struktura souborového systému
 * @param ptr ukazatel na data, která se mají zapsat
 * @param size velikost o jaké má probíhat zápis
 * @param count kolikrát se má provést zápis
 * @return počet bytů, které se zapsali
 */

size_t write_vfs(VFS **vfs, const void *ptr, size_t size, size_t count);

/**
 * @brief Funkce pro čtení ze souborového systému
 * @param vfs struktura souborového systému
 * @param ptr ukazatel na buffer, do kterého se mají data uložit
 * @param size velikost o jaké má probíhat čtení
 * @param count kolikrát se má provést čtení
 * @return počet bytů, které se načetli
 */

size_t read_vfs(VFS **vfs, void *ptr, size_t size, size_t count);

/**
 * @brief Funkce, pro vyprázdění bufferu pro zápis.
 * @param vfs struktura souborového systému
 */
void flush_vfs(VFS **vfs);

#endif
