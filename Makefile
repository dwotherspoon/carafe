CFLAGS=-Wall -Wextra -Wpedantic -O0
OBJ=main.o
CC=gcc
EXECNAME=bin/myapp-fcgi

all: main
	@echo "Carafe built..."

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(OBJ) -lfcgi

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)