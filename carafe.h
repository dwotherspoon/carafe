
/* Typedef for requests. */
typedef struct {
	int i;
} Request;

/* Typedef for responses. */
typedef struct {
	int i;
	uint8_t content_type;
} Response;

/* Typedef for route handlers (in controllers). */
typedef void (*Handler)(Request *, Response *);

/* Typedef for linked-list of routes. */
typedef struct _Route {
	char * pattern;
	Handler handler;
	struct _Route * next;
} Route;
