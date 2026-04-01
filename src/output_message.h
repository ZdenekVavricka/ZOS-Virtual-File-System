/**
 * @file general_headers.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s definovanými zprávami pro uživatele.
 * @version 1.0
 */

#ifndef FILE_SYSTEM_OUTPUT_MESSAGE_H
#define FILE_SYSTEM_OUTPUT_MESSAGE_H

/**
 * @brief Zpráva pokud, vše proběhlo v pořádku
 */
#define OK "OK\n"

/**
 * @brief Zpráva pokud, nebyl nalezen zdrojový soubor
 */
#define FILE_NOT_FOUND "FILE NOT FOUND\n"

/**
 * @brief Zpráva pokud nebyl nalezen cílový soubor
 */
#define PATH_NOT_FOUND "PATH NOT FOUND\n"

/**
 * @brief Zpráva pokud, již soubor nebo adresář existuje se stejným jménem ve stejném adresáři
 */
#define EXIST "EXIST\n"

/**
 * @brief Zpráva při mazání adresáře pokud neni prázdný
 */
#define NOT_EMPTY "NOT EMPTY\n"

/**
 * @brief Zpráva pokud symbolický link odkazuje na již neexistující soubor
 */
#define BROKEN_LINK "BROKEN LINK\n"

/**
 * @brief Zpráva pokud, nelze vytvořit soubor na pevném disku
 */
#define CANNOT_CREATE_FILE "CANNOT CREATE FILE\n"

#endif
