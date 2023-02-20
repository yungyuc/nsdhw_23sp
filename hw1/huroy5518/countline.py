#!/usr/bin/env python3

import sys
import os.path

if "PYTHON_BIN" in os.environ:
    pythonenv = os.environ["PYTHON_BIN"] 
    del os.environ["PYTHON_BIN"]
    ret = os.system("exec " + pythonenv + " " + " ".join(sys.argv))
    sys.exit(ret >> 8)

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
        sys.exit(255)
