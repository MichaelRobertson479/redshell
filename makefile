all: shell.o
	gcc -o program shell.o

shell.o: shell.c
	gcc -c shell.c

run:
	./program
