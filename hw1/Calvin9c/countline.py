#!/usr/bin/env ${PYTHON_BIN:-python}

# We add a Shebang line at the top of our script which can specifies the path to the bash interpreter.
# This allows us to include bash instructions in the same file as our python code and the script will be interpreted correctly.

import sys
import os.path

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))

# vim: set ff=unix fenc=utf8 ft=python et sw=4 ts=4 sts=4 tw=79:
