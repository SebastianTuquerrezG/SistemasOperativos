/**
 * @file
 * @brief Funciones de utilidad
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
*/

#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>


/** @brief Convierte una cadena en minusculas */
char * lcase(char *str);

/** @brief Convierte una cadena en mayusculas */
char * ucase(char *str);

/** @brief Verifica si dos cadenas de caracteres son iguales */
#define equals(x, y) (strcmp(x, y) == 0)


/**
 * @brief Encuentra la primera ocurrencia de alguno de set dentro de str.
 * @param str Cadena a revisar
 * @param set Caracteres a buscar
 * @return Primera ocurrencia de algun caracter en set.
*/
char * find_first_of(char * str, char * set);

/**
 * @brief Encuentra el primer caracter que o se encuentre en set dentro de str.
 * @param str Cadena a revisar
 * @param set Caracteres a buscar
 * @return Primera ocurrencia de algun caracter que no se encuentre en en set.
*/
char * find_first_not_of(char * str, char * set);

/**
 * @brief Recorta una cadena en el caracter especificado.
 * @param str Cadena a recortar
 * @param c Caracter a partir del cual se recorta la cadena
 * @param from_end Buscar c desde el final de la cadena.
 * @return Cadena recortada hasta el caracter anterior a c, seguido de nulo.
*/
char * truncate_to(char * str, char c, int from_end);


/**
 * @brief Extrae la extension de un archivo
 * @param path Ruta del archivo
 * @return extension del archivo
*/
char * get_extension(char * path);


/**
 * @brief Quita la extension de una ruta dada
*/
char * remove_extension(char* path);

/**
 * @brief Reemplaza la extension de un archivo por otra
 * @param path Ruta al archivo
 * @param oldext Extension original
 * @param newext Nueva extension
 * @return Ruta con la extension cambiada
*/
char * replace_extension(char* path, char * oldext, char * newext);

/**
 * @brief Reserva memoria y concatena dos cadenas de caracteres
 * @param str1 Primera cadena
 * @param str2 Segunda cadena
 * @return Nueva cadena que contiene str1 y str2 concatenadas.
*/
char * concat(char * str1, char * str2);

#ifndef strrev
/**
* @brief Invierte una cadena
* @param str Cadena a invertir
* @return Apuntador a la misma cadena con los caracteres en orden inverso
*/
char *strrev(char *str);
#endif


#endif
