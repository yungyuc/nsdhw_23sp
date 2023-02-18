#!/usr/bin/env python
import sys
import os.path
import os

python_bin = os.getenv('PYTHON_BIN', 'python3')
if os.system(python_bin + " --version >/dev/null 2>&1") != 0:
    sys.stderr.write("exec: {}: not found\n".format(python_bin))
    sys.exit(1)


if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    with open('service.py', 'w') as f:
        f.write("""#!/usr/bin/env python3

import sys
import os.path
import os



if len(sys.argv) < 2:
    sys.stdout.write('missing file name\\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        #lines = open(fname).readlines()
        sys.stdout.write('{} lines in {}\\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\\n'.format(fname))
        """)
    os.system(f"{os.getenv('PYTHON_BIN', 'python3')} {os.path.dirname(os.path.abspath(__file__))}/service.py {sys.argv[1]}")   
