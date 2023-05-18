CXXPROC = clang++
CPPFLAGS = -std=c++17 -O3 -Wall -shared -fPIC -m64 
PYFLAGS  = `python3 -m pybind11 --includes` `python3-config --includes --ldflags`# for debug
PROG_TARGET = matrix_test.py
LIB_TARGET = _matrix
MKLROOT=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKLROOT}/libmkl_def.so \
		${MKLROOT}/libmkl_avx2.so \
		${MKLROOT}/libmkl_core.so \
		${MKLROOT}/libmkl_intel_lp64.so \
		${MKLROOT}/libmkl_sequential.so \
		-lpthread -lm -ldl \
	    -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -L/opt/intel/lib/intel64 -L/opt/intel/mkl/lib/intel64 

NUMPY_INC = $(shell python3 -c "import numpy;")
MODULE_NAME = `python3-config --extension-suffix`
SO = _matrix$(MODULE_NAME)

all: ../mod.cpp
	$(CXXPROC) -I. -o $(SO) $< $(CPPFLAGS) -I/usr/include/mkl $(PYFLAGS) -I$(NUMPY_INC) ${LDFLAGS} 

.PHONY: test
test: _matrix.so
	env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v

.PHONY: clean
clean:
	rm -rf *.o *.so .pytest_cache/ __pycache__/
