
/* Typedef for requests. */
typedef struct {
	int i;
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
