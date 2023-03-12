#!/usr/bin/env python3

import sys
import os.path

PYTHON_BIN = os.environ['PYTHON_BIN'] if 'PYTHON_BIN' in os.environ.keys() else 'python3'

argv=f'argv = {str(sys.argv)}'
code = f"\"import sys\\n" + argv +  "\\nimport os.path\\nif len(argv) < 2:\\n    print(\'missing file name\')\\nelif len(argv) > 2:\\n    print(\'only one argument is allowed\\\n\')\\nelse:\\n    fname = argv[1]\\n    if os.path.exists(fname):\\n        with open(fname) as fobj:\\n            lines = fobj.readlines()\\n        print(\'{} lines in {}\\\n\'.format(len(lines), fname))\\n    else:\\n        print(\'{} not found\\\n\'.format(fname))\""

ret = os.system(f'echo {code} | exec {PYTHON_BIN}')
if ret != 0 : ret = 1
exit(ret)

