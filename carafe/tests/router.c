#include <lib/router.h>
#include <stdlib.h>


static void test_match(void) {
	Route root;
	route_init(&root);
	route_add(&root, "/const", (Handler)1, GET);
	route_add(&root, "/const", (Handler)2, GET);
	route_debug(&root, 0);
}

static void test_priority(void) {
	Route root;
	route_init(&root);
	route_add(&root, "/*", (Handler)1, GET);
	route_add(&root, "/@", (Handler)2, GET);
	route_add(&root, "/const", (Handler)3, GET);
	route_debug(&root, 0);
}


int main(void) {
	Route root;
	route_init(&root);
	
	route_add(&root, "/carafe/home", (Handler)1, GET | POST);
	route_debug(&root, 0);

	route_add(&root, "/carafe/blog/new", (Handler)2, GET | POST);
	route_debug(&root, 0);
	fflush(0);

	route_add(&root, "/carafe/blog/@", (Handler)3, GET | POST);
	route_debug(&root, 0);

	route_add(&root, "/carafe/*", (Handler)3, GET | POST);
	route_debug(&root, 0);

	route_add(&root, "/carafe/x/no", (Handler)3, GET | POST);
	route_debug(&root, 0); 
	//test_match();
	//test_priority();
	return 0;
}
