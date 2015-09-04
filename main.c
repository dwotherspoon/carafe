#include <sys/types.h>
#include <sys/stat.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <Judy.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include <carafe.h>
#include <routing.h>
#include <views.h>
#include <log.h>

/* http://judy.sourceforge.net/downloads/index.html */
/* http://www.aprelium.com/data/doc/2/abyssws-win-doc-html/cgivars.html */
extern char **environ;
FILE * pLog;


#define APP_NAME "test-app"

void handler_404(Request * req, Response * res) {
	puts("Oops...");
}

void setup(void) {
	/* Setup logging */
	log_init();
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

Response * build_response() {
	Response * r = malloc(sizeof(Response));
	char * key = "Content-Type";
	char * value = "text/html";
	void ** v;
	/* Initialise headers */
	r->headers = NULL;
	JSLI(v, r->headers, key);
	*v = value;
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
	char ** val = NULL;
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
		h = route_request(req);
		/* Apply the selected controller */
		if (h != NULL) {
			(*h)(req, res);
			puts(res->body);
		}
		else {
			//render_view(req, res);
		}
		/* Show the request */

		JSLF(val, res->headers, buf);
		while (val != NULL) {
			printf("%s: %s\n", buf, *val);
			JSLN(val, res->headers, buf);
		}
		puts("\n");
		print_debug(req);
		/* Do GC from this request */
		JSLFA( i , req->vars);
		free(req);
		free(res);
	}
	log_close();
	return -1;
}
