/**
 * @file server.c
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
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

#include "util.h"

#define BUFFER_SIZE 1024
#define MAX_FILENAME_LENGTH 256

/**
 * @brief Estructura para almacenar socket de cliente, y el archivo a tratar
 */
typedef struct{
	int socket;
	char file[MAX_FILENAME_LENGTH];
} paramsThread;

/**
 * @brief Manejador de señales.
 * @param num Numero de la señal recibida
 * @note El manejador debe ser instalado mediante sigaction(2).
 */
void signal_handler(int num);

/** 
 * @brief Funcion que ejecuta el hilo del cliente en el servidor
 * @param arg estructura de datos para el servidor
*/
void *clientThread(void *arg);

/** @brief Controla la ejecucion del servidor */
int finished = 0;

/**
 * @brief manda mensaje de error y sale de ejecucion del programa
 *
 * @param msg mensaje de error
 */
void error(const char *msg);

/**
 * @brief Obtiene el tamaño de un archivo
 * @param filename ruta del archivo
 * @return tamaño del archivo en bytes
 */
long get_file_size(char *filename);

/**
 * @brief Obtiene el tipo de contenido de un archivo
 * @param filename ruta del archivo
 * @return tipo de contenido
 */
const char *get_content_type(const char *filename);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Uso: %s <ip> <puerto>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	system("clear");

	//socket del servidor, socket de cliente, y el puerto del servidor
	int sockfd, clisockfd, puerto;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	char buffer[BUFFER_SIZE], *ip;
	pthread_t conex;
	paramsThread params;

	puerto = atoi(argv[2]);
	ip = argv[1];

	// Estructura de accion de señal
	struct sigaction act;

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

	// Establecer las propiedades del socket
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(puerto);
	if (inet_aton(ip, &serv_addr.sin_addr) == 0)
	{ // Escuchar en la direccion 	ip QUE SE ME HAYA ASIGNADO
		fprintf(stderr, "Unable to parse the address\n");
		error("inet_Aton addres");
	}

	// Vincular el socket a una dirección y puerto
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error en la vinculación\n");
	}

	// Escuchar conexiones entrantes
	if(listen(sockfd, 10) != 0){
		error("Error al escuchar conexiones\n");
	}
	printf("Servidor iniciado\n");

	while (!finished)
	{
		clilen = sizeof(cli_addr);
		// Aceptar la conexión entrante
		clisockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (clisockfd < 0)
		{
			error("Error al aceptar la conexión\n");
		}

		params.socket = clisockfd;

		ssize_t bytesReceived = recv(clisockfd, buffer, BUFFER_SIZE, 0);
		if (bytesReceived < 0)
		{
			perror("Error al recibir los datos\n");
			exit(EXIT_FAILURE);
		}

		buffer[bytesReceived] = '\0';
		printf("Petición recibida: %s\n", buffer);

		strncpy(params.file, buffer, MAX_FILENAME_LENGTH - 1);
		params.file[MAX_FILENAME_LENGTH - 1] = '\0';

		char filepath[256] = "./www";
		strncat(filepath, buffer, 250);
		printf("%s\n", filepath);
		if (access(filepath, F_OK) != 0)
		{
			// Obtener la fecha actual
			long filesize = get_file_size(filepath);
			time_t current_time;
			time(&current_time);
			struct tm *time_info = localtime(&current_time);
			char date_str[80];
			strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S %Z", time_info);

			// Mostrar información adicional en la consola del cliente
			char response[BUFFER_SIZE];
			snprintf(response, BUFFER_SIZE, "HTTP/1.0 404 Not Found\r\n"
											"X-Powered-By: OS HTTP Server\r\n"
											"Content-Type: %s; charset=utf-8\r\n"
											"Content-Length: %ld\r\n"
											"Date: %s\r\n"
											"\r\n"
											"<!DOCTYPE html>"
											"<html lang="" en "">"
											"<head>"
											"<meta charset=""utf-8"">"
											"<title>Error</title>"
											"</head>"
											"<body>"
											"<pre>Cannot GET %s</pre>"
											"</body>"
											"</html>",
											get_content_type(buffer), filesize, date_str, filepath);
			ssize_t bytesSent = send(clisockfd, response, strlen(response), 0);
			if (bytesSent < 0)
			{
				perror("Error al enviar los datos");
				pthread_exit(NULL);
			}
			close(clisockfd);
			continue;
		}		

		// Crear el hilo y pasar el descriptor de socket del cliente
		if (pthread_create(&conex, NULL, clientThread, (void *)&params) != 0)
		{
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}
	close(sockfd);
	printf("Servidor cerrado");

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

void *clientThread(void *arg){
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;
	paramsThread *params = (paramsThread *)arg;
	const char *filename = params->file;
	char filepath[256] = "./www";
	strncat(filepath, filename,250);
	FILE *file = fopen(filepath,"rb");
	if(file == NULL){
		perror("Error al abrir el archivo");
		send(params->socket, "HTTP/1.0 500 Internal Server Error\r\n\r\n", 36, 0);
		close(params->socket);
		pthread_exit(NULL);
	}

	long filesize = get_file_size(filepath);
	if (filesize < 0)
	{
		perror("Error al obtener el tamaño del archivo");
		send(params->socket, "HTTP/1.0 500 Internal Server Error\r\n\r\n", 36, 0);
		fclose(file);
		close(params->socket);
		pthread_exit(NULL);
	}

	// Obtener la fecha actual
	time_t current_time;
	time(&current_time);
	struct tm *time_info = localtime(&current_time);
	char date_str[80];
	strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S %Z", time_info);

	// Mostrar información adicional en la consola del cliente
	char response[BUFFER_SIZE];
	snprintf(response, BUFFER_SIZE, "HTTP/1.0 200 OK\r\n"
									"X-Powered-By: OS HTTP Server\r\n"
									"Content-Type: %s; charset=utf-8\r\n"
									"Content-Length: %ld\r\n"
									"Date: %s\r\n"
									"\r\n", 
									get_content_type(filename), filesize, date_str);
	ssize_t bytesSent = send(params->socket, response, strlen(response), 0);
	if (bytesSent < 0)
	{
		perror("Error al enviar los datos");
		fclose(file);
		close(params->socket);
		pthread_exit(NULL);
	}

	while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
	{
		ssize_t bytesSent = send(params->socket, buffer, bytesRead, 0);
		if (bytesSent < 0)
		{
			error("Error al enviar los datos");
		}
	}
	printf("Archivo enviado correctamente");


	fclose(file);
	close(params->socket);
	pthread_exit(NULL);
}

const char *get_content_type(const char *filename)
{
	const char *extension = strrchr(filename, '.');
	if (extension != NULL)
	{
		if (strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0)
		{
			return "text/html";
		}
		else if (strcmp(extension, ".txt") == 0)
		{
			return "text/plain";
		}
		else if (strcmp(extension, ".css") == 0)
		{
			return "text/css";
		}
		else if (strcmp(extension, ".js") == 0)
		{
			return "application/javascript";
		}
		else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0)
		{
			return "image/jpeg";
		}
		else if (strcmp(extension, ".png") == 0)
		{
			return "image/png";
		}
		else if (strcmp(extension, ".gif") == 0)
		{
			return "image/gif";
		}
		else if (strcmp(extension, ".pdf") == 0)
		{
			return "application/pdf";
		}
		else
		{
			return "application/octet-stream";
		}
	}
	return "application/octet-stream";
}
long get_file_size(char *filename)
{
	struct stat st;
	if (stat(filename, &st) == 0)
	{
		return st.st_size;
	}
	return -1; // Error al obtener el tamaño del archivo
}