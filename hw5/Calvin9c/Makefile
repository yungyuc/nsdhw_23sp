CXXFLAGS=-O3 -Wall -m64 -shared -std=c++17 -fPIC
MKL_INCLUDE=-I /usr/include/mkl
PYTHON_VERSION=$(shell python3 -c "import sys; print('{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_EXT=`python3-config --extension-suffix`
PYBIND_INCLUDE=`python3 -m pybind11 --includes`
PYTHON_INCLUDE=`python3-config --includes`
SRC=_matrix

.PHONY: all test clean

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=-L${MKL_LIB_DIR} -lmkl_def -lmkl_avx2 -lmkl_core -lmkl_intel_lp64 -lmkl_sequential

all: $(SRC)$(PYTHON_EXT)

$(SRC)$(PYTHON_EXT): ../mod.cpp
	clang++ $(CXXFLAGS) $(MKL_INCLUDE) $(PYBIND_INCLUDE) $(PYTHON_INCLUDE) $< $(MKL_LIBS) -o $@

test: all
	python3 -m pytest -v

clean:
	rm -rf *$(PYTHON_EXT) __pycache__ .pytest_cache 