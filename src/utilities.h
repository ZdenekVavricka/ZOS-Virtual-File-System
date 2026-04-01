/**
 * @file utilities.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s pomocnými funkcemi.
 * @version 1.0
 */

#ifndef FILE_SYSTEM_UTILITIES_H
#define FILE_SYSTEM_UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "string.h"
#include "structures.h"

/**
 * @brief Funkce pro kontrolu, zdali existuje zadaný soubor.
 * @param filename jméno/cesta k souboru
 * @return true pokud soubor existuje
 *         false pokud soubor neexistuje
 */
bool file_exists (char *filename);

/**
 * @brief Funkce pro porovnání dvou stringů.
 * @param string1 string1
 * @param string2 string1
 * @return true pokud se stringy shodují
 *         false pokud se stringy neshodují
 */
bool str_equals(char *string1, char *string2);

/**
 * @brief Funkce pro zjištění zdali string něco obsahuje
 * @param string string
 * @return true pokud je string prázdný
 *         false pokud string něco obsahuje
 */
bool str_empty (char *string);

/**
 * @brief Funkce pro zjištění velikosti formátu souborového systému v bytech
 * @param string string, obsahující velikost formátu
 * @return požadovanou velikost formátu souborového systému v bytech
 */
int32_t get_size_from_string(char *string);

/**
 * @brief Funkce pro načtení řádky z terminálu.
 * @return line - načtená řádka z terminálu
 */
char *get_line();

/**
 * @brief Funkce pro kontrolu formátování souboru.
 * @param vfs informace souborového systému
 * @return true pokud je souborový systém naformátovaný
 *         false pokud není souborový systém naformátovaný
 */
bool control_format_of_VFS(VFS **vfs);

/**
 * @brief Funkce pro oddělení poslední části stringu, podle oddělovače
 * @param string vstupní string
 * @param second_part poslední část stringu
 * @param split_char oddělovací znak
 * @return first_part počáteční část stringu
 */
char *str_trim_and_split(char *string, char **second_part, char split_char);

/**
 * @brief Funkce pro odstranění lomeno n ze stringu
 * @param line vstupní string
 * @return string bez lomeno n
 */
char *remove_slash_n(char *line);

/**
 * @brief Funkce pro zjištění velikosti vstupního souboru
 * @param file vstupní soubor
 * @return size velikost vstupního souboru
 */
int get_size_of_input_file(FILE *file);

/**
 * @brief Funkce pro výpis a ukončení podle návratových hodnot definovaných v souboru return_values.h.
 * @param error číselná hodnota návratové hodnoty
 */
void error_print(int error);
#endif
