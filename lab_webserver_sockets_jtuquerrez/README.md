Elaborado por Joan Sebastian Tuquerrez Gomez
Construir dos programas en C, que actuarán como cliente y servidor de archivos estaticos via el protocolo HTTP.

El programa servidor deberá recibir por línea de comandos el puerto en el cual va a recibir conexiones.
Para cada conexión recibida, creará un hilo que se encargará de enviar el archivo solicitado por el cliente.
Una vez enviado el archivo, se deberá cerrar la conexión con el cliente y terminar.

El formato de una solicitud HTTP es el siguiente:

     GET /ruta_archivo HTTP/1.1

Los archivos del servidor se encuentran en el subdirectorio www.

El servidor deberá buscar el archivo solicitado y enviarlo de vuelta al cliente, con un mensaje con el siguiente formato:

HTTP/1.1 200 OK
X-Powered-By: OS HTTP Server
Content-Type: TIPO_ARCHIVO
Content-Length: TAMANO_ARCHIVO
Date: FECHA_ACTUAL

Seguido de dos caracteres de fin de línea, y seguido se enviará el contenido del archivo.



Cuando un archivo no se encuentra, el servidor deberá construir y enviar el siguiente mensaje:

HTTP/1.1 404 Not Found
X-Powered-By: OS HTTP Server
Content-Type: text/html; charset=utf-8
Content-Length: TAMANO_RESPUESTA
Date: Wed, FECHA_ACTUAL
Connection: close

<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot GET /ruta_archivo</pre>
</body>
</html>


El programa cliente se conectará a la dirección IPv4 del servidor y el puerto correspondiente 
(ambos argumentos de línea de comandos.). Después de conectarse, recibirá del usuario el nombre de un archivo a obtener.
Con este nombre de archivo deberá crear y enviar una solicitud tipo GET al servidor con el siguiente formato:

    GET /ruta_archivo HTTP/1.1

Seguida de dos caracteres de fin de línea.

El cliente deberá procesar los encabezados enviados por el servidor hasta encontrar dos caracteres de fin de línea.
Si la respuesta es (200 OK), deberá guardar el contenido del archivo que se obtiene en el mensaje del servidor a 
un directorio "files/".

Asegurarse de tener creada la carpeta "files/" en la misma direccion de los archivos de servidor y cliente

Para la ejecucion del servidor se ejecuta el siguiente comando:
    ./server ip puerto 
    
Para la ejecucion del cliente se ejecuta el siguiente comando
    ./client ip_del_servidor puerto_del_servidor