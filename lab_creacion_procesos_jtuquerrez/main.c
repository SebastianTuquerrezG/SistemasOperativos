/**
 * @file main.c
 * @author Sebastian Tuquerrez (jtuquerrez@unicauca.edu.co)
 * @brief 
 * @version 1.0.0
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "split.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief  Prueba de la funcion split
 */
void test_split();

int main(int argc, char *argv[])
{
    char linea[BUFSIZ];
    int finished = 0;
    int len;
    pid_t pid;
    split_list *list;
    while (!finished)
    {
        printf("$ ");
        if (fgets(linea, BUFSIZ, stdin) == NULL)
        {
            finished = 1;
            continue;
        }
        // post: linea leida

        len = strlen(linea);
        if (len <= 1)
        {
            continue;
        }

        // post: len >1
        if (linea[len - 1] == '\n')
        {
            linea[len - 1] = '\0';
            len--;
        }

        // post: linea sin ENTER al final
        // Ignorar comentarios
        if (linea[0] == '#')
        {
            continue;
        }
        // Terminar si el comando es exit o quit o logout
        if (strcmp(linea, "exit") == 0 || strcmp(linea, "quit") == 0 || strcmp(linea, "logout")==0)
        {
            exit(0);
        }

        // post: linea no es comentario ni exit ni quit
        // Crear un proceso hijo
        pid = fork();
        if (pid != 0)
        {
            // post: proceso padre
            // Esperar a que termine el proceso hijo
            waitpid(pid, 0, 0);
            // post: proceso hijo terminado
        }
        else
        {
            // post: proceso hijo
            // Ejecutar el comando
            list = split(linea, " \t\n|");
            execvp(list->parts[0], list->parts);
            // Exec solo retorna cuando ocurrio un error
            // Imprimir el error generado
            perror(list->parts[0]);
            // Terminar proceso hijo
            exit(1);
        }
    }
}

void test_split()
{
    split_list *list;
    list = split(" hola mundo esta es una cadena     adios", " \t\n|");

    for (int i = 0; i < list->count; i++)
    {
        printf("#%s# ", list->parts[i]);
    }
}
