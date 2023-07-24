/**
 * @file main.c
 * @author Sebastian Tuquerrez (jtuquerrez@unicauca.edu.co)
 * @brief Busqueda de archivos
 * @version 1.0.0
 * @date 2023-03-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <dirent.h>

/**
 * @brief Imprime ayuda de como se usa el programa
 *
 * @param program_name Nombre del archivo ejecutable
 */
void usage(char *program_name);

/**
 * @brief Determina si una ruta dada es un directorio o no
 *
 * @param ruta Ruta del directorio a verificar
 * @return int 1 si la ruta es un directorio valido, 0 en caso contratio
 */
int es_directorio(char *ruta);

/**
 * @brief Busca recursivamente archivos/directorios cuyo nombre tenga patron que es dado
 *
 * @param directorio Directorio base de busqueda
 * @param patron Texto dentro del nombre del archivo/directorio a buscar
 * @return int numero de elementos encontrados en la busqueda
 */
int buscar(char *directorio, char *patron);

int main(int argc, char *argv[])
{
    char *dir;
    char *pattern;
    int total;

    if (argc != 3)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    // POST: se han pasado tres argumentos al programa
    //  argv[0] = ejecutable
    //  argv[1] = directorio
    //  argv[2] = patron

    dir = argv[1];
    pattern = argv[2];

    if (!es_directorio(dir))
    {
        fprintf(stderr, "El directorio %s no existe\n", dir);
        exit(EXIT_FAILURE);
    }
    // POST: dir es un directorio valido
    total = buscar(dir, pattern);

    if (total > 0)
        printf("Se encontraron %d archivos/directorios con el patron %s en %s\n", total, pattern, dir);
    else
        printf("No se encontro %s dentro de %s\n", pattern, dir);
    exit(EXIT_SUCCESS);
}

void usage(char *program_name)
{
    printf("Uso del programa:\n");
    printf("\t%s DIR PATTERN\n", program_name);
    printf("\t\tBusca archivos/directorios cuyo nombre contiene PATTERN dentro de DIR de forma recursiva\n");
}

int es_directorio(char *ruta)
{
    struct stat st;
    if (stat(ruta, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
            return 1;
    }
    return 0;
}

int buscar(char *directorio, char *patron)
{
    DIR *dir;
    dir = opendir(directorio);
    int total = 0;
    struct dirent *ent;

    if (dir == NULL)
    {
        perror("Error al abrir el directorio");
        exit(EXIT_FAILURE);
    }

    while ((ent = readdir(dir)) != NULL)
    {
        char path [PATH_MAX+1];
        char *ruta = (char *)malloc(strlen(realpath(directorio,path)) + strlen(ent->d_name + 2));
        strcpy(ruta, directorio);
        strcat(ruta, "/");
        strcat(ruta, ent->d_name);
        if (strstr(ent->d_name, patron) != NULL)
        {
            printf("%s\n", realpath(ruta,path));
            total = total + 1;
        }

        if (es_directorio(ruta) && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            total = total + buscar(ruta, patron);

        free(ruta);
    }
    closedir(dir);
    return total;
}