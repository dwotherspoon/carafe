/* This file is automagically generated, 
I highly recommend you do not edit directly 
and instead edit the corresponding view. */

#include "../carafe.h"
#include "default.html.h"
#include <fcgi_stdio.h>

/* Declarations */

vfunc default_html_vfuncs[] = {
	&default_html_func0,
	&default_html_func1,
	&default_html_func2
};

int i = 0;

/* Void functions */

void default_html_func0(Response * r) {
	printf("i %d", i++);
}


void default_html_func1(Response * r) {
	puts("default_html_func1");
}

void default_html_func2(Response * r) {
	puts("default_html_func2");
}

