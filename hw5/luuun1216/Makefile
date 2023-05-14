.PHONY:clean all test target

NUMPY_PATH := $(shell python3 -c "import numpy; print(numpy.get_include())")
MKLLIB=-I/usr/include/mkl
PYTHON=python3

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so

CXX=clang++
CXXFLAGS=-std=c++14 -O3 -Wall -Wextra -fPIC -shared -m64

all: target

test: matrix.*.so
	python3 -m pytest -v

target: ../mod.cpp
	$(CXX) $(CXXFLAGS) -I./ -I$(NUMPY_PATH) $(MKL_LIBS) $(MKLLIB) `$(PYTHON) -m pybind11 --includes` $< -o _matrix`$(PYTHON)-config --extension-suffix` -I /usr/include/python3.8 -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm
clean:
	$(RM) -r $(TARGET) *.so *.txt