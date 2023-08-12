/**
 * @file client.c
 * @brief Servidor HTTP
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @author Joan Sebastian Tuquerrez Gomez <jtuquerrez@unicauca.edu.co>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>

#include "util.h"
#include "split.h"

#define h_addr h_addr_list[0]
#define MAX_FILENAME_LENGTH 256
#define BUFFER_SIZE 1024

/**
 * @brief Manejador de señales.
 * @param num Numero de la señal recibida
 * @note El manejador debe ser instalado mediante sigaction(2).
 */
void signal_handler(int num);

/** @brief Controla la ejecucion del servidor */
int finished = 0;

/**
 * @brief manda mensaje de error y sale de ejecucion del programa
 *
 * @param msg mensaje de error
 */
void error(const char *msg);

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Uso: %s <ip> <puerto>\n", argv[0]);
	}
	//socket del servidor, puerto a conectarse
	int sockfd, puerto;
	FILE *file;
	//ip del servidor
	char *ip;
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;

	puerto = atoi(argv[2]);
	ip = argv[1];

	struct sigaction act;

	struct sockaddr_in serv_addr;
	struct hostent *server;

	// Inicializar la estructura con ceros (NULL)
	memset(&act, 0, sizeof(struct sigaction));

	// Establecer el manejador de la señal
	act.sa_handler = signal_handler;

	// Instalar el manejador de la señal SIGINT
	sigaction(SIGINT, &act, NULL);

	// Instalar el manejador de la señal SIGTERM
	sigaction(SIGTERM, &act, NULL);

	// Crear un socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Error al abrir el socket");
	}

	// Obtener la dirección IP del servidor (localhost en este caso)
	server = gethostbyname(ip);
	if (server == NULL)
	{
		error("Error al obtener la dirección del servidor");
	}

	// Establecer las propiedades del socket
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(puerto);
	if (inet_pton(AF_INET, ip, &(serv_addr.sin_addr)) <= 0)
	{
		perror("inet_pton");
	}

	// Conectar al servidor
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error al establecer la conexión");
	}
	printf("Conectado al servidor\n");

	char filename[MAX_FILENAME_LENGTH];
	char fileSend[MAX_FILENAME_LENGTH];
	printf("Ingrese el comando de solicitud: \n");

	fgets(filename, MAX_FILENAME_LENGTH, stdin);	
	split_list *result = split(filename, " ");
	// Eliminar el salto de línea
	strcpy(fileSend, result->parts[1]);
	fileSend[strcspn(fileSend, "\n")] =	'\0'; // Eliminar el salto de línea

	char request[BUFFER_SIZE];
	snprintf(request, BUFFER_SIZE, "%s", fileSend);

	// Enviar el nombre del archivo al servidor
	ssize_t bytesSent = send(sockfd, request, strlen(request), 0);
	if (bytesSent < 0)
	{
		perror("Send error");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	// Abrir un archivo en modo escritura para recibir los datos
	char response[BUFFER_SIZE];
	char *outuputFile = malloc(BUFFER_SIZE);
	snprintf(outuputFile, BUFFER_SIZE, "./files%s", fileSend);

	file = fopen(outuputFile, "w");
	if (file == NULL)
	{
		error("Error al crear el archivo");
	}
	
	free(outuputFile);

	// Recibir y escribir los datos en el archivo
	while ((bytesRead = recv(sockfd, response, BUFFER_SIZE, 0)) > 0)
	{
		fwrite(response, sizeof(char), bytesRead, file);

		write(STDOUT_FILENO, response, bytesRead);
	}
	if (bytesRead < 0)
	{
		perror("Recv error");
		exit(EXIT_FAILURE);
	}

	// Cerrar el archivo
	// Liberar la memoria asignada por la función split
	for (int i = 0; i < result->count; i++)
	{
		free(result->parts[i]);
	}
	fclose(file);
	free(result);

	// Cerrar la conexión del socket
	close(sockfd);
	printf("El archivo se ha recibido correctamente.\n");

	return 0;
}

void error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void signal_handler(int num)
{
	printf("Signal %d received\n", num);
	finished = 1;
}
