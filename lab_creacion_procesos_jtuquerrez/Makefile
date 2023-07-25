CC=gcc

all: main.o split.o
	$(CC) -o main main.o split.o

main.o: main.c split.h
	$(CC) -c -o main.o main.c

split.o: split.c split.h
	$(CC) -c -o split.o split.c

clean:
	rm -rf *.o main doc

doc:
	doxygen