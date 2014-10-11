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

def process_mustache():
	return None

def compile_view(f):
	#declarations
	decs = list()
	#void functions
	funcs = list()
	#functions which return char*
	funcse = list()

	r = open(f, "r")
	w = open(f.replace("/views/", "/_views/"), "w")
	cur = r.readline()
	#scan file for mustaches
	while (cur != ""):
		if "{{" in cur:
			match = "" + cur
			# Read lines until we have a closing tag
			while (not "}}" in match):
				temp = r.readline()
				if (temp == ""):
					print "Failed to compile view " + f + ": unbalanced {{ }} statement."
					return -1
				else:
					match += temp
			#if "{{" in match and  "}}" in match:
			start = match.index("{{")
			end = match.index("}}")
			#Pull out surounds
			w.write(match[0:start])
			w.write("{{func_22}}")
			w.write(match[end+2:])
			match = match[start:end+2]
			print match
		else:
			w.write(cur)
		cur = r.readline()
	w.close()

compile_view("./views/default.html")