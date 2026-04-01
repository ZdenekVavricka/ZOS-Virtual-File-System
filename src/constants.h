/**
 * @file constants.h
 * @author Zdeněk Vavřička
 * @brief Hlavičkový soubor s definovanými konstantami.
 * @version 1.0
 */

#ifndef FILE_SYSTEM_CONSTANTS_H
#define FILE_SYSTEM_CONSTANTS_H

/**
 * @brief Definice délky názvu souborového systému
 */
#define SIGNATURE_LENGTH 20

/**
 * @brief Definice délky popis souborového systému
 */
#define VOLUME_DESCRIPTOR_LENGTH 20

/**
 * @brief Definice názvu souborového systému
 */
#define SUPER_BLOCK_SIGNATURE "===== ext ====="

/**
 * @brief Definice popisu souborového systému
 */
#define VOLUME_DESCRIPTOR "VFS version 1.0"


/**
 * @brief Definice velikosti datového bloku
 * TESTING = 32
 * DEFAULT = 256
 * MAX = 1024
 */
#define CLUSTER_SIZE 256

/**
 * @brief Násobitel velikosti datového bloku při formátování, pro velikost FS > 1GB.
 */
#define GB_MULTIPLIER 1000

/**
 * @brief Násobitel velikosti datového bloku při formátování, pro velikost FS > 1 MB.
 */
#define MB_MULTIPLIER 100

/**
 * @brief Definice minimální velikosti souborového systému
 */
#define MIN_FS_SIZE 20000

/**
 * @brief Definice maximální velikosti souborového systému
 */
#define MAX_FS_SIZE 2000000000

/**
 * @brief Definice počtu i-nodů v souborového systému
 * DEFAULT 256
 */
#define DEFAULT_NUMBER_OF_INODES 256

/**
 * @brief Definice počtu přímých odkazů
 */
#define NUM_OF_DIRECT_LINK 5

/**
 * @brief Definice počátečního typu uloženého v i-nodu
 */
#define NULL_TYPE (-1)

/**
 * @brief Definice typu souboru uloženého v i-nodu
 */
#define FILE_TYPE 0

/**
 * @brief Definice typu adresář uloženého v i-nodu
 */
#define DIRECTORY_TYPE 1

/**
 * @brief Definice typu symbolický link uloženého v i-nodu
 */
#define SLINK_TYPE 2

/**
 * @brief Definice velikosti struktury directory_item
 */
#define DIRECTORY_ITEM_SIZE 16

/**
 * @brief Definice maximálního počtu struktur dir_item v jednom datovém bloku
 */
#define MAX_DIR_ITEMS_IN_BLOCK (CLUSTER_SIZE/DIRECTORY_ITEM_SIZE)

/**
 * @brief Definice délky názvu souboru
 */
#define FILENAME_LENGTH 12

/**
 * @brief Definice počáteční hodnoty pro nastavení i-nodu
 */
#define ID_ITEM_FREE (-1)

/**
 * @brief Definice počáteční velikosti vstupního souboru
 */
#define DEFAULT_SIZE_OF_INPUT_FILE (-1)

/**
 * @brief Definice počátečního počtu prvků
 */
#define LACK_OF_FREE_ELEMENTS (-1)

/**
 * @brief Definice oddělovače částí zpráv
 */
#define COMMAND_DELIMETER " "

/**
 * @brief Definice oddělovače částí cesty k souboru
 */
#define PATH_DELIMETER "/"

/**
 * @brief Definice délky vstupního stringu.
 */
#define LINE_LENGTH 512

/**
 * @brief Definice návratové hodnoty při nezdaření konzerve stringu na číslo
 */
#define NUM_RETURN_FAILED (-10)

/**
 * @brief Definice maximálního počtu datových bloků pro jeden i-node
 */
#define MAX_DATA_BLOCKS_FOR_INODE (((CLUSTER_SIZE / 4) * (CLUSTER_SIZE / 4)) + (CLUSTER_SIZE / 4) + 5)

/**
 * @brief Definice maximální velikosti souboru.
 */
#define MAX_FILE_SIZE (MAX_DATA_BLOCKS_FOR_INODE * CLUSTER_SIZE)

#endif
