CFLAGS=-Wall -Wextra -Wpedantic -O0
OBJ=main.o _views/manifest.o carafe/routing.o carafe/views.o _views/default.html.o
CC=gcc
EXECNAME=bin/myapp-fcgi

all: main
	@echo "Carafe application built..."

main: $(OBJ)
	$(CC) -o $(EXECNAME) $(OBJ) -lfcgi -lJudy

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)
