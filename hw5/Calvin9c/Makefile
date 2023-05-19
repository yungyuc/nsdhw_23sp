CXXFLAGS=-O3 -Wall -m64 -shared -std=c++17 -fPIC
MKL_INCLUDE=-I /usr/include/mkl
PYTHON_VERSION=$(shell python3 -c "import sys; print('{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_CONFIG=`python3-config --extension-suffix`
PYBIND_INCLUDE=`python3 -m pybind11 --includes`
SRC=_matrix

.PHONY: all test clean

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so

all: $(SRC)$(PYTHON_CONFIG)

$(SRC)$(PYTHON_CONFIG): ../mod.cpp
	clang++ $(CXXFLAGS) -I ./ $(MKL_INCLUDE) $(PYBIND_INCLUDE) $< -o $@ -I/usr/include/python$(PYTHON_VERSION) ${MKL_LIBS} 

test: all
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache 