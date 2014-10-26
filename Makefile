CFLAGS=-Wall -Wextra -Wpedantic -O0
OBJ=main.o carafe/routing.o carafe/views.o
CC=gcc
EXECNAME=bin/myapp-fcgi

all: main
	@echo "Carafe application built..."

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(OBJ) -lfcgi -lJudy

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)