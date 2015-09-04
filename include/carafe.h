
/* Typedef for requests. */
typedef struct {
	int i;
	void * vars;
} Request;

/* Typedef for responses. */
typedef struct {
	int i;
	void * headers;
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
#define GET			0x1
#define POST		0x2
#define PUT			0x4
#define PATCH		0x8
#define DELETE 		0x10
#define COPY		0x20
#define HEAD 		0x40
#define OPTIONS 	0x80
#define LINK 		0x100
#define UNLINK 		0x200
#define PURGE		0x400
