Carafe
======

A C web framework, or at least some part of one.

Compiles to a FastCGI application, using LibJudy for associative arrays.

Views
-----
Views use a moustache-like syntax, allowing C code to be embedded within HTML. At compile-time moustaches are processed by a python script and C files created. At run-time all views are loaded into memory and a look-up table used to execute moustaches within the view.

Routing
-----
TBD, no wild card matching or parameterisation yet...
