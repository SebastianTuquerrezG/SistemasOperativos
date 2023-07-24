all: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c -o main.o main.c

clean: 
	rm -rf main.o main doc

doc:
	doxygen