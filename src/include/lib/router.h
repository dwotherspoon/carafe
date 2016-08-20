#include <carafe.h>

typedef struct route_child_t {
	struct route_t *pRoute;
	struct route_child_t *pNext;     
} RouteChild;


/* Typedef for trie for routes. */
typedef struct route_t {
	char * match;
	RouteChild *pChildren;

	/* Methods are only used by leaf nodes */
	int methods;
	Handler handler;
	
} Route;

void route_init(Route *root);
void route_debug(Route *root, int level);
void route_add(Route *root, const char *pattern, Handler handler, int methods);
Handler route_request(Route *root, Request *req);
