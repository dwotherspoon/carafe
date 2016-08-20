#include <sys/types.h>
#include <sys/stat.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include <lib/router.h>
#include <carafe.h>
#include <views.h>
#include <log.h>

/* http://www.aprelium.com/data/doc/2/abyssws-win-doc-html/cgivars.html */
extern char **environ;
FILE * pLog;
Route root;

void handler_404(Request * req, Response * res) {
	puts("Oops...");
}

void setup(void) {
	/* Setup logging */
	log_init();
	/* Load views into memory */
	load_views();
	/* Router */
	route_init(&root);
	route_add(&root, "/carafe/home", &handler_404, GET | POST);
	route_add(&root, "/carafe/blog/new", &handler_404, GET | POST);
	route_add(&root, "/carafe/blog/@", &handler_404, GET | POST);
	route_add(&root, "/carafe/blog/*", &handler_404, GET | POST);
	route_add(&root, "/carafe/*", &handler_404, GET | POST);
	/* Bootstrap your routes in here (First is first to be matched)*/
	//route_add("/carafe/404", &handler_404, GET | UNLINK);
}

Request * build_request() {
	int i, l;
	char *key, *value;
	Request * r = malloc(sizeof(Request));
	hashtable_init(&(r->vars), 32);

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

		hashtable_set(&(r->vars), key, l + 1, value);
		/* Free the key, hashtable has it's own copy. */
		free(key);
	}
	return r;
}

Response * build_response() {
	Response * r = malloc(sizeof(Response));
	/* Initialise headers */
	hashtable_init(&(r->headers), 8);
	hashtable_set(&(r->headers), "Content-Type", sizeof("Content-Type"), "text/html");
	hashtable_set(&(r->headers), "X-Powered-By", sizeof("X-Powered-By"), "Carafe");
	return r;
}

void print_debug(Request * req) {
	puts("<div id=\"carafe-debug\">\nServed by Carafe over FastCGI<br />\r\n");
	hashtable_iter_first(&(req->vars));
	do {
		printf("{ %s => %s }<br />\n", hashtable_iter_key(&(req->vars)), hashtable_iter_value(&(req->vars)));
	} while(hashtable_iter_next(&(req->vars)));
	puts("</div><pre>");
	//hashtable_debug(&(req->vars));
	route_debug(&root, 0);
	puts("</pre>");
}

int main(void) {
	int i;
	char buf[256];
	Request * req;
	Response * res;
	Handler h;
	setup();
	log_puts("Setup complete.\n");
	/* request loop */
	while (FCGI_Accept() >= 0) {
		gets(buf);
		//puts("Content-type: text/html\n\n");
		req = build_request();
		res = build_response();
		/* Show the request */
		hashtable_iter_first(&(res->headers));
		do {
			printf("%s: %s\r\n", hashtable_iter_key(&(res->headers)), hashtable_iter_value(&(res->headers)));
		} while (hashtable_iter_next(&(res->headers)));
		putchar('\r');
		putchar('\n');
		print_debug(req);
		puts("<pre>");
		h = route_request(&root, req);
		puts("</pre>");
		if (h) {
			(*h)(req, res);
		}
		else {
			puts("I'm all out of routes...");
		}

		/* Do GC from this request */
		hashtable_free(&(req->vars), NULL);
		hashtable_free(&(res->headers), NULL);
		free(req);
		free(res);
	}
	log_close();
	return -1;
}
