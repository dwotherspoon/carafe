#include "../carafe.h"
#include <stdlib.h>
#include <Judy.h>
#include <fcgi_stdio.h>
#include <assert.h>
#include "routing.h"


Route * routes_head;
Route * routes_tail;

/* Hopefully this parses to a jump table for some speed */
int parse_method(char * method) {
	int m = (method[0] << 0x10) | (method[1] << 0x08) | (method[2]);
	/* Be fast, only check first three chars */
	switch (m) {
		case 0x474554:
			return GET;
			break;
		case 0x504F53:
			return POST;
			break;
		case 0x505554:
			return PUT;
			break;
		case 0x504154:
			return PATCH;
			break;
		case 0x44454C:
			return DELETE;
			break;
		case 0x434F50:
			return COPY;
			break;
		case 0x484541:
			return HEAD;
			break;
		case 0x4F5054:
			return OPTIONS;
			break;
		case 0x4C494E:
			return LINK;
			break;
		case 0x554E4C:
			return UNLINK;
			break;
		case 0x505552:
			return PURGE;
			break;
		default:
			return 0;
			break;
	}
}

/* Test if a request matches a pattern */
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
	/* we hit null/EoS? Match'd. */
	return (!*uri && !*pattern);
}


Handler route_request(Request * req) {
	Route * cur = routes_head;
	char ** uri, ** method;
	int meth;
	assert(req != NULL);
	JSLG(uri, req->vars, "REQUEST_URI");
	JSLG(method, req->vars, "REQUEST_METHOD");
	meth = parse_method(*method);
	/* Iterate over linked list, pick first match TODO: Check REQUEST_METHOD */
	while (cur != NULL) {
		if ((meth & cur->methods) && route_test(*uri, cur->pattern)) {
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
	r->methods = methods;
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
