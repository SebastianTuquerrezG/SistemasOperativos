all: disk_image main.o
	gcc -o hdisk main.o

test: all
	./hdisk mbr_disk_0
	./hdisk mbr_disk_1


disk_image:
	sudo dd if=/dev/sda of=mbr_disk_0 bs=512 count=1
	sudo dd if=/dev/sdb of=mbr_disk_1 bs=512 count=1
	sudo chown ingesis.ingesis mbr_disk_0 mbr_disk_1

main.o: main.c
	gcc -c -o main.o main.c

doc:
	doxygen

clean:
	rm -f *.o hdisk mbr_disk_* docs/
