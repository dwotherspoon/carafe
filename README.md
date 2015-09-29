Carafe
======

A C web framework, or at least some parts of one.

Compiles down to a FastCGI application, includes a simple hash table implementation.

Views
-----
Views use a moustache-like syntax, allowing C code to be embedded within HTML. At compile-time moustaches are processed by a python script and C files created. At run-time all views are loaded into memory and a look-up table used to execute moustaches within the view.

Routing
-----
TBD, no wild card matching or parameterisation yet...

Building
-----
Requires libfcgi-dev.

Hosting
-----
My recommended web server for hosting is lighttpd. Below is an example configuration (you will need to enable mod_rewrite and mod_fastcgi):

  url.rewrite-once = ("^/carafe/(.+)$" => "/carafe/")

  fastcgi.server = (
    "/carafe/" => ((
      "bin-path" => "/path/to/carafe/bin/myapp-fcgi",
      "socket" => "/tmp/carafe.socket",
    ))
  )

Note that the carafe folder must exist or you'll just get a 404 error.
If you don't want to enable mod_rewrite, setting the server.error-handler-404 to the path carafe is being served on is equivalent.
