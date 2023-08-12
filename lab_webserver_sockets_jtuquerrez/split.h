/**
 * @file
 * @brief String split library
 * @author Erwin Meza Vega <emezav@gmail.com> 
*/

#ifndef SPLIT_H_
#define SPLIT_H_

/** @brief Cantidad maxima de partes de una cadena */
#define MAX_PARTS 255

/**
 * @brief Lista de partes (palabras) de una cadena.
 * Define la estructura de la lista de palabras obtenida como valor de retorno
 * de la función split.
 */
typedef struct {
    char * parts[MAX_PARTS]; /*!< Arreglo de subcadenas */
    int count; /*!< Cantidad de subcadenas*/
}split_list;

/** 
 * @brief Divide la cadena en subcadenas
 * @param str Cadena a dividir
 * @param delim Cadena que contiene los delimitadores
 * @return Lista de subcadenas.
 * Divide una cadena en palabras, usando los delimitadores especificados
 * o los delimitadores por defecto
 */
split_list * split(char * str, const char * delim);

#endif
