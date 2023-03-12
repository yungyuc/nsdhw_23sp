#!/usr/bin/env python3

import sys
import os
import subprocess

PyBin = os.environ['PYTHON_BIN']

try:
    subprocess.check_output([PyBin, '--version'], stderr=subprocess.STDOUT)
except (subprocess.CalledProcessError, OSError):
    sys.stderr.write('exec: {}: not found'.format(PyBin))
    sys.exit(1)

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
