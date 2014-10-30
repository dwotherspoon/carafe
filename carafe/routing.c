#include "../carafe.h"
#include <stdlib.h>
#include <Judy.h>
#include <fcgi_stdio.h>
#include <assert.h>
#include "routing.h"


Route * routes_head;
Route * routes_tail;

/* Test if a route matches a pattern */
int route_test(char * uri, char * pattern) {
	while(*uri && *pattern) {
		if (*uri == *pattern) {
			uri++;
			pattern++;
		}
		else {
			return 0;
		}
	}
	//we hit null/EoS? Match'd.
	return (!*uri && !*pattern);
}

Handler route_request(Request * req) {
	Route * cur = routes_head;
	char ** uri;
	assert(req != NULL);
	JSLG(uri, req->vars, "REQUEST_URI");
	/* Iterate over linked list, pick first match */
	while (cur != NULL) {
		/* some routing logic */
		if (route_test(*uri, cur->pattern)) {
			return cur->handler;
		}
		cur =  cur->next;
	}
	return NULL;
}

void route_add(char * p, Handler h, int methods) {
	Route * r = malloc(sizeof(Route));
	r->pattern = p;
	r->handler = h;
	r->next = NULL;
	if (routes_head == NULL) {
		routes_head = r;
		routes_tail = r;
	}
	else {
		routes_tail->next = r;
		routes_tail = r;
	}
}
