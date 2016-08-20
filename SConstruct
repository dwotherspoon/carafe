build_flags = {
	'CFLAGS' : ['-Wall', '-Wextra', '-Wpedantic', '-O0']
}

import os, sys

env = Environment(ENV = os.environ, tools = ['default', 'textfile'])

for (k, v) in build_flags.items():
    env[k] = v

Export('env')

# Build Carafe itself.
SConscript('src/SConscript')

# Build the user's application with carafe.
SConscript('app/SConscript')