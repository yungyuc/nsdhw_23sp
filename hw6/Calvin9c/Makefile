CXXFLAGS=-O3 -Wall -m64 -shared -std=c++14 -fPIC
MKL_PATH=-I /usr/include/mkl

PYTHON_CONFIG := `python3-config --extension-suffix`
# PYTHON_INCLUDE := $(shell $(PYTHON_CONFIG) --includes)

PYBIND11_INCLUDE := `python3 -m pybind11 --includes`

src=_matrix

.PHONY: all test clean

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so

all:
	clang++ $(CXXFLAGS) ${MKL_LIBS} ${MKL_PATH} $(PYBIND11_INCLUDE) _matrix.cpp -o $(src)$(PYTHON_CONFIG) -I/usr/include/python3.10 

test: all
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache 