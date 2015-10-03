#include <lib/router.h>
#include <stdlib.h>

static void test_insert_empty(void) {
	Route root;
	route_init(&root);
	route_add(&root, "/const", (Handler)1, GET);
	route_debug(&root, 0);

}

static void test_insert_head(void) {
	Route root;
	route_init(&root);
	route_add(&root, "/@", (Handler)1, GET);
	route_add(&root, "/const", (Handler)2, GET);
	route_debug(&root, 0);
}

static void test_match(void) {
	Route root;
	route_init(&root);
	route_add(&root, "/const", (Handler)1, GET);
	route_add(&root, "/const", (Handler)2, GET);
	route_debug(&root, 0);
}

static void test_wildcard(void) {

}

static void test_parameter(void) {

}

int main(void) {
	/*
	Route root;
	route_init(&root);
	route_debug(&root, 0);
	route_add(&root, "/carafe/home", (Handler)1, GET | POST);
	route_add(&root, "/carafe/blog/new", (Handler)2, GET | POST);
	route_add(&root, "/carafe/blog/@", (Handler)3, GET | POST);
	route_add(&root, "/carafe/*", (Handler)3, GET | POST);
	route_add(&root, "/carafe/x/no", (Handler)3, GET | POST);
	route_debug(&root, 0); */
	//test_insert_empty();
	//test_insert_head();
	test_match();
	return 0;
}
