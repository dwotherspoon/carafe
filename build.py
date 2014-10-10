#!/usr/bin/env python
import re, os, sys
'''
This file is used to compile your application and carafe into a single runtime.

Compiling views:
-- Pull every {{ }} out
-- Build c file with contents in. Name by file and line number..
-- Build array of procedures (one for {{= }} and one for  {{ }})
-- Replace {{ }} with ID token ({{=24}} or {{25}})
-- Build table of view functions (in header?)
-- Special constructs for FOR loops. And dictionaries.
-- Special constructs for declaring view-global vars.
'''

controllers = ["controllers/default.c"]

views = ["views/default.html"]

def compile_view(f):
	#declarations
	decs = list()
	#just functions
	funcs = list()
	#functions which return char*
	funcse = list()

	r = open(f, "r")
	w = open(f.replace("/views/", "/_views/"), "w")
	cur = r.readline()
	while (cur != ""):
		w.write(cur)
		cur = r.readline()
	w.close()

compile_view("./views/default.html")