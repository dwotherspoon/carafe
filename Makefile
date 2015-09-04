CFLAGS=-Wall -Wextra -Wpedantic -O0 -I include/
OBJ=main.o _views/manifest.o carafe/routing.o carafe/views.o carafe/log.o _views/default.html.o 
CC=clang
EXECNAME=bin/myapp-fcgi

all: main
	@echo "Carafe application built..."

main: $(OBJ)
	$(CC) -o $(EXECNAME) $(OBJ) -lfcgi -lJudy

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)
