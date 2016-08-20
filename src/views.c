#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <carafe.h>
#include "../_views/manifest.h"
#include <views.h>

/* Load views into memory for speed */
void load_views(void) {
	uint i;
	FILE *fp;
	long sz;
	size_t pEnd, nLen;
	char buf[PATH_MAX];

	populate_views();
	getcwd(buf, PATH_MAX);
	/* n.b. we assume the bin folder... */
	pEnd = strlen(buf) - 3; 
	strncpy(buf + pEnd, "_views/\0", 8);

	/* e is offset to terminating 0 */
	pEnd += 7;
	for (i = 0; i < VIEW_COUNT; i++) {
		nLen = strlen(views[i].name);
		strncpy(buf + pEnd, views[i].name, nLen);
		/* Terminate the string */
		*(buf + pEnd + nLen) = '\0';
		/* Now read in the file */
		/* TODO: Check file exists */
		fp = fopen(buf, "r");
		/* Get the size of the view.. */
		fseek(fp, 0, SEEK_END);
		sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		views[i].body = malloc(sz * sizeof(char));
		fread(views[i].body, sizeof(char), sz, fp);
		fclose(fp);
	}
}

/* Function to workout what to do with moustaches */
void process_moustache(char * start, int len) {
	int fnum = atoi(start + 4);
	(*views[0].vfuncs[fnum])(NULL);
}


void render_view(Request * req, Response * res) {
	char * cur, * close;

	cur = strstr(views[0].body, "{{");
	fwrite(views[0].body, sizeof(char), (cur - views[0].body), stdout);
	while (cur) {
		close = strstr(cur, "}}");
		/* Next moustache */
		process_moustache(cur + 2, close - cur - 2);
		cur = strstr(cur + 2, "{{");
		close += 2;
		/* More moustaches, or EOF? */
		if (cur) {
			fwrite(close, sizeof(char), (cur - close), stdout);
		}
		else {
			puts(close);
		}
	}
	/* Does this view have a cleanup function defined? */
	if (views[0].cleanup != NULL) {
		(*views[0].cleanup)();
	}
}