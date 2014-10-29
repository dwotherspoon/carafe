#include <stdint.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <Judy.h>
#include <time.h>
#include <string.h>
#include "carafe.h"
#include "carafe/routing.h"
#include "carafe/views.h"

/* http://judy.sourceforge.net/downloads/index.html */
/* http://www.aprelium.com/data/doc/2/abyssws-win-doc-html/cgivars.html */
extern char **environ;
Response * r = NULL;

char a[] = "TEST";

/* Bootstrap your routes in here */
void setup(void) {
	/* Load views into memory */
	load_views(); 
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
	}
	return r;
}

int main(void) {
	int i;
	char buf[256];
	char * res;
	time_t tstart;
	Request * req;
	setup();
	/* request loop */
	while (FCGI_Accept() >= 0) {
		time(&tstart);
		/*
		build_request();
		route_request();
		build_response();
		send_response();
		*/
		gets(buf);
		puts("Content-type: text/html\n\n");
		req = build_request();
		/*
		puts("<form action='' method='post'>");
			puts("<input type='text' name='test' />");
		puts("</form>");

		for (i = 0; environ[i]; i++) {
			printf("%s<br />\n", environ[i]);
		}
		printf("Input:<br />\n%s", buf);
		//JSLG(res, arry, "HTTP_VERB");
		//printf("JUDY: %s", verb); */
		render_view(req, r);

		print_debug(req);
		/* GC from this request */
		JSLFA( i , req->vars);
		free(req);
	}
	return -1;
}

void print_debug(Request * req) {
	char sstr[256] = {'\0'};
	char ** val = NULL;
	time_t tdiff;
	puts("<div id=\"carafe-debug\">\nServed by Carafe over FastCGI<br />\n\r");
	//printf("Request took %f secs", difftime(s, time(&tdiff)));
	JSLF(val, req->vars, sstr);
	while (val != NULL) {
		printf("{ %s => %s }<br />\n", sstr, *val);	
		JSLN(val, req->vars, sstr);
	}
	puts("</div>");


}