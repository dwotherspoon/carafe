
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
typedef char * (*vfunce)(Response *);

/* Typedef for linked-list of routes. */
typedef struct _Route {
	char * pattern;
	Handler handler;
	struct _Route * next;
} Route;
