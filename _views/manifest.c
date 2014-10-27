#include "../carafe.h"
#include "manifest.h"
#include "default.html.h"
#include <stdlib.h>
#include <string.h>
#include <fcgi_stdio.h>

View views[VIEW_COUNT];

void populate_views(void) {
	/* default.html */
	views[0].name = malloc(13 * sizeof(char));
	strncpy(views[0].name, "default.html\0", 13);
	views[0].vfuncs = default_html_vfuncs;
	views[0].cleanup = NULL;
	/* And the next.. */
}