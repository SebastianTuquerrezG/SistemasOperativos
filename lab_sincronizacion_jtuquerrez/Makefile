all: prodcons.o philosophers.o
	gcc -o ./prodcons/prodcons ./prodcons/prodcons.o -pthread
	gcc -o ./philosophers/philosophers ./philosophers/philosophers.o -pthread

prodcons.o: ./prodcons/prodcons.c
	gcc -c -o ./prodcons/prodcons.o ./prodcons/prodcons.c

philosophers.o: ./philosophers/philosophers.c
	gcc -c -o ./philosophers/philosophers.o ./philosophers/philosophers.c

doc:
	doxygen

clean:
	rm -f ./philosophers/*.o ./prodcons/*.o ./prodcons/prodcons ./philosophers/philosophers
	rm -rf docs/