#include <lib/router.h>
#include <stdlib.h>

int main(void) {
	Route root;
	route_init(&root);
	route_debug(&root, 0);
	route_add(&root, "/carafe/home", (Handler)1, GET | POST);
	route_add(&root, "/carafe/blog/new", (Handler)2, GET | POST);
	route_add(&root, "/carafe/blog/:id", (Handler)3, GET | POST);
	route_add(&root, "/carafe/*", (Handler)3, GET | POST);
	route_debug(&root, 0);
	return 0;
}
