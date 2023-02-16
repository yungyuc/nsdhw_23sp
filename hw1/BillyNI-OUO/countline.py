#!/usr/bin/env python3

import sys
import os.path
import os


if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    os.system(f"{os.getenv('PYTHON_BIN', 'python3')} service.py {sys.argv[1]}")   
