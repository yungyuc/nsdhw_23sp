
PY_INCLUDE=`python3 -m pybind11 --includes`
PY_CFG=`python3-config --extension-suffix` `python3-config --includes --ldflags`
NUMPY_INCLUDE=$(shell python3 -c "import numpy; print(numpy.get_include())")
MKL_PATH = /usr/lib/x86_64-linux-gnu/libmkl_rt.so
SO_NAME=_matrix


.PHONY: clean all

all: SO_NAME

SO_NAME: ../mod.cpp matrix.hpp
	g++ -O3 -Wall -shared -std=c++17 -I. -I${NUMPY_INCLUDE} -fPIC ${PY_INCLUDE} $< -o ${SO_NAME}${PY_CFG} ${MKL_PATH}

test: SO_NAME
	python3 -m pytest -v 

clean:
	rm -f *.so
	rm -rf __pycache__
	rm -rf .pytest_cache

