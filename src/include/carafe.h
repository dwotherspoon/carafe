#include <lib/hashtable.h>

#ifndef CARAFE_H
#define CARAFE_H

/* Typedef for requests. */
typedef struct {
	int i;
	HashTable vars;
} Request;

/* Typedef for responses. */
typedef struct {
	int i;
	HashTable headers;
	char * body;
} Response;

/* Typedef for route handlers (in controllers). */
typedef void (*Handler)(Request *, Response *);

/* Typedef for view functions... */
typedef void (*vfunc)(Response *);
typedef void (*gcfunc)(void);

/* Typedef for views and associated functions */
typedef struct {
	char * name;
	char * body;
	vfunc * vfuncs;
	gcfunc cleanup;
} View;

/* Defines for HTTP methods */
#define GET			1 << 0
#define POST		1 << 1
#define PUT			1 << 2
#define PATCH		1 << 3
#define DELETE 		1 << 4
#define COPY		1 << 4
#define HEAD 		1 << 5
#define OPTIONS 	1 << 6
#define LINK 		1 << 7
#define UNLINK 		1 << 8
#define PURGE		1 << 9

#endif
