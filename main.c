#include <stdint.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <Judy.h>
#include "carafe.h"
#include "carafe/routing.h"
#include "carafe/views.h"
#include "_views/manifest.h"

/* http://judy.sourceforge.net/downloads/index.html */
/* http://www.aprelium.com/data/doc/2/abyssws-win-doc-html/cgivars.html */
extern char **environ;
void * arry = NULL;
char verb[] = "POSTGETETC";
Response * r = NULL;

/* Bootstrap your routes in here */
void setup(void) {
	void * v = verb;
	/* Load views into memory */
	load_views(); 
	JSLI(v, arry, "HTTP_VERB");	
	r = malloc(sizeof(Response));
}

int main(void) {
	int i;
	char buf[256];
	char * res;
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
		render_view(NULL, r);

		print_debug();
	}
	return -1;
}

void print_debug(void) {
	puts("Served by Carafe over FastCGI<br />\n\r");
	printf("%lu views avaliable<br />\n\r", sizeof(view_names)/sizeof(void *));
}