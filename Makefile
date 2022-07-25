CC = gcc -O3

all: flute-net flute-ckt rand-pts

flute-net: flute-net.c flute.o
	$(CC) -o flute-net flute-net.c flute.o 

flute-ckt: flute-ckt.c flute.o \
	bookshelf_IO.c bookshelf_IO.h memAlloc.c memAlloc.h
	$(CC) -o flute-ckt flute-ckt.c flute.o bookshelf_IO.c memAlloc.c -lm

flute.o: flute.c flute.h
	$(CC) -c -o flute.o flute.c 

rand-pts: rand-pts.c
	$(CC) -o rand-pts rand-pts.c 

clean:
	rm -f *.o core*
