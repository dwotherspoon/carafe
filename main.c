#include <stdint.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <Judy.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "carafe.h"
#include "carafe/routing.h"
#include "carafe/views.h"

/* http://judy.sourceforge.net/downloads/index.html */
/* http://www.aprelium.com/data/doc/2/abyssws-win-doc-html/cgivars.html */
extern char **environ;
Response * r = NULL;

void handler_404(Request * req, Response * res) {
	puts("OKAY MOTO");
}

void setup(void) {
	/* Load views into memory */
	load_views();
	/* Bootstrap your routes in here (First is first to be matched)*/
	route_add("/carafe/404", &handler_404, GET | UNLINK);
}

Request * build_request() {
	int i, l;
	void ** v;
	char * key, * value;
	Request * r = malloc(sizeof(Request));
	r->vars = NULL;
	/* Populate the Environment vars */
	for (i = 0; environ[i]; i++) {
		value = environ[i];
		/* Find the start of the key */
		while (*value != '=') { value++; }
		l = value - environ[i];
		key = malloc(sizeof(char) * l + 1);
		strncpy(key, environ[i], l);
		key[l] = '\0';
		value++;
		JSLI(v, r->vars, key);
		*v = value;
		/* We can free this because Judy copies it */
		free(key);
	}
	return r;
}

void print_debug(Request * req) {
	char sstr[256] = {'\0'};
	char ** val = NULL;
	puts("<div id=\"carafe-debug\">\nServed by Carafe over FastCGI<br />\n\r");
	JSLF(val, req->vars, sstr);
	while (val != NULL) {
		printf("{ %s => %s }<br />\n", sstr, *val);
		JSLN(val, req->vars, sstr);
	}
	puts("</div>");
}

int main(void) {
	int i;
	char buf[256];
	char * res;
	Request * req;
	Handler h;
	setup();
	/* request loop */
	while (FCGI_Accept() >= 0) {
		/*
		build_request();
		route_request();
		build_response();
		send_response();
		*/
		gets(buf);
		puts("Content-type: text/html\n\n");
		req = build_request();
		h = route_request(req);
		if (h != NULL) {
			(*h)(req, r);
		}
		else {
			render_view(req, r);
		}

		print_debug(req);
		/* Do GC from this request */
		JSLFA( i , req->vars);
		free(req);
	}
	return -1;
}
