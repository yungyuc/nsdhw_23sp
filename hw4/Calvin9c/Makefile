CXXFLAGS=-O3 -Wall -m64 -shared -std=c++11 -fPIC
PYTHON_INCLUDES=$(shell python3 -m pybind11 --includes) $(shell python3-config --includes)
PYTHON_EXT_SUFFIX=$(shell python3-config --extension-suffix)
SRC=_matrix

.PHONY: all test clean

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=-L${MKL_LIB_DIR} -lmkl_def -lmkl_avx2 -lmkl_core -lmkl_intel_lp64 -lmkl_sequential -lpthread -lm -ldl -lblas -lmkl_intel_thread -liomp5

all:
	clang++ $(CXXFLAGS) -I/usr/include/mkl $(PYTHON_INCLUDES) $(SRC).cpp -o $(SRC)$(PYTHON_EXT_SUFFIX) $(MKL_LIBS)

test: all
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache