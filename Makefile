CFLAGS=-Wall -Wextra -Wpedantic -O0 -I carafe/include/
tests: CFLAGS += -DTESTS

OBJ=_views/manifest.o carafe/main.o carafe/views.o carafe/log.o _views/default.html.o 
LIB_OBJ=carafe/lib/router.o carafe/lib/hashtable.o
CC=clang
EXECNAME=bin/myapp-fcgi

all: main tests
	@echo "Carafe application built..."

main: $(OBJ) $(LIB_OBJ)
	$(CC) -o $(EXECNAME) $(OBJ) $(LIB_OBJ) -lfcgi

tests: test_router

test_router:
	$(CC) $(CFLAGS) -o bin/test-router carafe/tests/router.c carafe/lib/router.c carafe/lib/hashtable.c

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)
