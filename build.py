#!/usr/bin/env python
import os, sys
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

warning = '''/* This file is automagically generated, 
I highly recommend you do not edit directly 
and instead edit the corresponding view. */'''

''' Args: d = list of declarations, f = list of void functions, 
	m = string to process.'''
def process_mustache(d, f, m):
	#I wish this was a case statement :(
	if m[0] == "d":
		d.append(m[1:].strip())
		#declares are removed from the file.
		return "" 
	elif m[0] == " ":
		f.append(m[1:].strip())
		return "{{func"+str(len(f)-1)+"}}"
	else:
		return "Unknown specifier: " + m[0]

''' Args: d = list of declarations, f = list of void functions,
	fe = list of functions which return char*, fn = file name for view.'''
def build_source(d, f, fn):
	prefix = fn.split("/")[-1]
	fn = fn.replace("/views/", "/_views/")
	w = open(fn+".c", 'w')
	w.write(warning+"\n\n")
	w.write("/* Declarations */\n\n")
	w.write("#include \"../carafe.h\"\n")
	w.write("#include \""+prefix+".h\"\n")
	w.write("#include <fcgi_stdio.h>\n\n")
	#declarations
	for dec in d:
		w.write(dec+"\n")
	w.write("\n/* Void functions */\n\n")
	prefix = prefix.replace(".", "_")
	#void functions
	for i in range(0, len(f)):
		w.write("void " + prefix + "_func" + str(i) + "(Response * r) {\n")
		w.write(f[i]+"\n")
		w.write("}\n\n")
	w.close()


def compile_view(f):
	#declarations
	decs = list()
	#void functions
	funcs = list()

	r = open(f, "r")
	w = open(f.replace("/views/", "/_views/"), "w")
	cur = r.readline()
	#scan file for mustaches
	while (cur != ""):
		if "{{" in cur:
			match = "" + cur
			# Read lines until we have a closing tag
			while not "}}" in match:
				temp = r.readline()
				#test for EOF
				if (temp == ""):
					print "Failed to compile view " + f + ": unbalanced {{ }} statement."
					return -1
				else:
					match += temp
			#if "{{" in match and  "}}" in match:
			start = match.index("{{")
			end = match.index("}}")
			#Pull out surrounds
			w.write(match[0:start])
			w.write(process_mustache(decs, funcs, match[start+2:end]))
			w.write(match[end+2:])
			match = match[start:end+2]
		else:
			w.write(cur)
		cur = r.readline()
	print "decs: " + str(decs)
	print "funcs: " + str(funcs)
	w.close()
	build_source(decs, funcs, f)

def create_manifest(f):
	print "..."

compile_view("./views/default2.html")