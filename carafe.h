
/* Typedef for requests. */
typedef struct {
	int i;
	void * vars;
} Request;

/* Typedef for responses. */
typedef struct {
	int i;
	char * body;
} Response;

/* Typedef for route handlers (in controllers). */
typedef void (*Handler)(Request *, Response *);

/* Typedef for view functions... */
typedef void (*vfunc)(Response *);
typedef void (*gcfunc)(void);

/* Typedef for linked-list of routes. */
typedef struct _Route {
	char * pattern;
	int methods;
	Handler handler;
	struct _Route * next;
} Route;

/* Typedef for views and associated functions */
typedef struct {
	char * name;
	char * body;
	vfunc * vfuncs;
	gcfunc cleanup;
} View;

/* Defines for HTTP methods */
#define GET			1
#define POST		2
#define PUT			4
#define PATCH		8
#define DELETE 	16
#define COPY		32
#define HEAD 		64
#define OPTIONS 128
#define LINK 		256
#define UNLINK 	512
#define PURGE		1024
