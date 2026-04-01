/**
 * @file commands.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s definovanými příkazy.
 * @version 1.0
 */

#ifndef FILE_SYSTEM_COMMANDS_H
#define FILE_SYSTEM_COMMANDS_H

/**
 * @brief Definice jednotlivých příkazů
 */
#define CP "cp"
#define MV "mv"
#define RM "rm"
#define MKDIR "mkdir"
#define RMDIR "rmdir"
#define LS "ls"
#define CAT "cat"
#define CD "cd"
#define PWD "pwd"
#define INFO "info"
#define INCP "incp"
#define OUTCP "outcp"
#define LOAD "load"
#define FORMAT "format"
#define SLINK "slink"
#define DEBUG "debug"
#define EXIT "exit"

/**
 * @brief Funkce, která rozhodne o jaký příkaz se jedná
 * @param vfs struktura souborového systému
 * @param line načtený příkaz od uživatele
 * @return true pokud vše probíhá v pořádku
 *         false pokud uživatel zadá příkaz exit
 */
int process_command(VFS **vfs, char *line);

/**
 * @brief Funkce pro vykonání příkazu cp
 * @param vfs struktura souborového systému
 * @param src_path zdrojová cesta
 * @param dest_path cílová cesta
 */
void cp_command(VFS **vfs, char *src_path, char *dest_path);

/**
 * @brief Funkce pro vykonání příkazu mv
 * @param vfs struktura souborového systému
 * @param src_path zdrojová cesta
 * @param dest_path cílová cesta
 */
void mv_command(VFS **vfs, char *src_path, char *dest_path);

/**
 * @brief Funkce pro vykonání příkazu rm
 * @param vfs struktura souborového systému
 * @param string_path cesta k souboru
 */
void rm_command(VFS **vfs, char *string_path);

/**
 * @brief Funkce pro vykonání příkazu mkdir
 * @param vfs struktura souborového systému
 * @param string_path cesta k adresáři
 */
void mkdir_command(VFS **vfs, char *string_path);

/**
 * @brief Funkce pro vykonání příkazu rmdir
 * @param vfs struktura souborového systému
 * @param string_path cesta k adresáři
 */
void rmdir_command(VFS **vfs, char *string_path);

/**
 * @brief Funkce pro vykonání příkazu ls
 * @param vfs struktura souborového systému
 * @param path cesta k adresáři
 */
void ls_command(VFS **vfs, char *path);

/**
 * @brief Funkce pro vykonání příkazu cat
 * @param vfs struktura souborového systému
 * @param path cesta k souboru
 */
void cat_command(VFS **vfs, char *path);

/**
 * @brief Funkce pro vykonání příkazu cd
 * @param vfs struktura souborového systému
 * @param path cesta k adresáři
 */
void cd_command(VFS **vfs, char *path);

/**
 * @brief Funkce pro vykonání příkazu pwd
 * @param vfs struktura souborového systému
 */
void pwd_command(VFS **vfs);

/**
 * @brief Funkce pro vykonání příkazu info
 * @param vfs struktura souborového systému
 * @param path cesta k souboru
 */
void info_command(VFS **vfs, char *path);

/**
 * @brief Funkce pro vykonání příkazu incp
 * @param vfs struktura souborového systému
 * @param src_path zdrojová cesta
 * @param dest_path cílová cesta
 */
void incp_command(VFS **vfs, char *src_path, char *dest_path);

/**
 * @brief Funkce pro vykonání příkazu outcp
 * @param vfs struktura souborového systému
 * @param src_path zdrojová cesta
 * @param dest_path cílová cesta
 */
void outcp_command(VFS **vfs, char *src_path, char *dest_path);

/**
 * @brief Funkce pro vykonání příkazu load
 * @param vfs struktura souborového systému
 * @param path cesta k souboru
 */
void load_command(VFS **vfs, char *path);

/**
 * @brief Funkce pro vykonání příkazu format
 * @param vfs struktura souborového systému
 * @param vfs_size
 */
void format_command(VFS **vfs, int32_t vfs_size);

/**
 * @brief Funkce pro vykonání příkazu slink
 * @param vfs struktura souborového systému
 * @param src_path zdrojová cesta
 * @param dest_path cílová cesta
 */
void slink_command(VFS **vfs, char *src_path, char *dest_path);

/**
 * @brief Funkce pro vykonání příkazu debug
 * @param vfs struktura souborového systému
 */
void debug_command(VFS **vfs);
#endif
