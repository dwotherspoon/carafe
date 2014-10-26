#include <stdlib.h>
#include <fcgi_stdio.h>
#include "../carafe.h"
#include "views.h"

void render_view(Request * req, Response * res) {
	FILE *fp;
	long sz;

	fp = fopen("/home/david/Repos/carafe/_views/default.html", "r");
	fseek(fp, 0, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	res->body = malloc(sz*sizeof(char));
	fread(res->body, sizeof(char), sz, fp);
	puts(res->body);
	free(res->body);
	res->body = NULL;
	fclose(fp);
}