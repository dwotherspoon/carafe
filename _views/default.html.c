/* This file is automagically generated, 
I highly recommend you do not edit directly 
and instead edit the corresponding view. */

#include "../carafe.h"
#include "default.html.h"

vfunc default_html_funcs = {
	&default_html_func0,
}

vfunce default_html_funces = {
	&default_html_funce0,
	&default_html_funce1,
}


/* Declarations */

int i = 0;

/* Void functions */

void default_html_func0(Response * r) {
i++;
}

/* Char * functions */

char * default_html_funce0(Response * r) {
return "Test";
}

char * default_html_funce1(Response * r) {
return "This is a multi line"\
		"tag. Hopefully we can deal"\
		"with this...";
}

