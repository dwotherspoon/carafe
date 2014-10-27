#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <fcgi_stdio.h>
#include "../carafe.h"
#include "../_views/manifest.h"
#include "views.h"

char * view_cache[VIEW_COUNT];

/* Load views into memory for speed */
void load_views(void) {
	uint i;
	FILE *fp;
	long sz;
	size_t pEnd;
	size_t nLen;
	char * buf = malloc(PATH_MAX * sizeof(char));

	getcwd(buf, PATH_MAX);
	/* n.b. we assume the bin folder... */
	pEnd = strlen(buf) - 3; 
	strncpy(buf + pEnd, "_views/\0", 8);
	/* e is offset to terminating 0 */
	pEnd += 7;
	for (i = 0; view_names[i]; i++) {
		nLen = strlen(view_names[i]);
		strncpy(buf + pEnd, view_names[i], nLen);
		/* Terminate the string */
		*(buf + pEnd + nLen) = '\0';
		/* Now read in the file */
		/* TODO: Check file exists */
		fp = fopen(buf, "r");
		/* Get the size of the view.. */
		fseek(fp, 0, SEEK_END);
		sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		view_cache[i] = malloc(sz * sizeof(char));
		fread(view_cache[i], sizeof(char), sz, fp);
		fclose(fp);
	}
	free(buf);
}


void render_view(Request * req, Response * res) {
	puts(view_cache[0]);
}