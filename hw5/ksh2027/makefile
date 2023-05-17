CLANG = clang++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC -m64
PYFLAGS = `python3 -m pybind11 --includes` `python3-config --includes --ldflags`
MODULE_NAME = `python3-config --extension-suffix`
NUMPY_INC = $(shell python3 -c "import numpy; print(numpy.get_include())")
HEADERS = -I/usr/include/mkl
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIB_SO=libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MKL_LIBS=$(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) \
		-lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

# libmkl_def.so: default library for linking with MKL
# libmkl_avx2.so: optimized for AVX2 instruction set
# libmkl_core.so: core library for all functions in MKL
# libmkl_intel_lp64.so: Intel LP64 interface library
# libmkl_sequential.so: sequential library
# -lpthread: POSIX threads library
# -lm: math library
# -ldl: dynamic linking library
# -lblas: Basic Linear Algebra Subprograms (BLAS) library
# -lmkl_intel_lp64: Intel LP64 interface library
# -lmkl_intel_thread: Intel thread library
# -lmkl_core: core library for all functions in MKL
# -liomp5: Intel OpenMP runtime library

SO = _matrix$(MODULE_NAME)

.PHONY: test clean

all: $(SO)

_matrix.so: ../mod.cpp
	$(CLANG) -I. -o $@ $< $(CFLAGS) $(HEADERS) $(PYFLAGS) -I$(NUMPY_INC) ${MKL_LIBS}

test: $(SO)
	python3 -m pytest -v -s

clean:
	rm -f *.so
	rm -rf __pycache__ .pytest_cache