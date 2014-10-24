#include "carafe.h"
#include <fcgi_stdio.h>
#include "stdlib.h"
/* http://judy.sourceforge.net/downloads/index.html */
/* http://www.aprelium.com/data/doc/2/abyssws-win-doc-html/cgivars.html */
extern char **environ;

void setup(void) {

}

int main(void) {
	int i;
	char buf[256];
	while (FCGI_Accept() >= 0) {
		gets(buf);
		puts("Content-type: text/html\n\n");
		for (i = 0; environ[i]; i++) {
			printf("%s<br />\n", environ[i]);
		}
		printf("Input:<br />\n%s", buf);
	}

	return -1;
}