all: server.o client.o split.o util.o
	gcc -o server server.o split.o util.o -pthread
	gcc -o client client.o split.o util.o -pthread

server.o: server.c
	gcc -c -o server.o server.c

client.o: client.c
	gcc -c -o client.o client.c

split.o: split.c
	gcc -c -o split.o split.c

util.o: util.c
	gcc -c -o util.o util.c

doc:
	doxygen

clean:
	rm -rf docs/ *.o server client