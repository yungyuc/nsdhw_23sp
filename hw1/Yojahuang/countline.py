#!/usr/bin/env python

import sys
import os

pythonBin = os.popen('printenv PYTHON_BIN').read()
pythonBin = pythonBin.strip()

if os.system("{} --version > /dev/null 2>&1".format(pythonBin)) != 0:
    sys.exit("exec: {}: not found".format(pythonBin))

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

