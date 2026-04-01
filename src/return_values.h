/**
 * @file return_values.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s návratovými hodnotami.
 * @version 1.0
 */
#ifndef FILE_SYSTEM_RETURN_VALUES_H
#define FILE_SYSTEM_RETURN_VALUES_H

/**
 * @brief Obecná návratová hodnota.
 */
#define GENERAL_ERROR 150

/**
 * @brief Návratová hodnota při špatném předání parametrů.
 */
#define FAILED_TO_PASS_PARAMETERS_ERROR 151

/**
 * @brief Návratová hodnota pokud se nepodařila alokovat paměť
 */
#define FAILED_TO_ALLOCATE_MEMORY_ERROR 152

/**
 * @brief Návratová hodnota pokud se nepodařilo načíst ze souboru/konzole
 */
#define FAILED_TO_READ_ERROR 153

/**
 * @brief Návratová hodnota pokud se nepodařilo převést string na číslo.
 */

#define STRING_NUMBER_CONVERSION_ERROR 154

/**
 * @brief Návratová hodnota pokud se nepodařilo otevřít soubor.
 */

#define FAILED_TO_OPEN_FILE 155

#endif