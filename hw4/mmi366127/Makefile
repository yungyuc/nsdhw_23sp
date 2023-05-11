PY_INCLUDE=`python3 -m pybind11 --includes`
PY_CFG=`python3-config --extension-suffix` `python3-config --includes --ldflags`
MKL_PATH = /usr/lib/x86_64-linux-gnu/libmkl_rt.so
SO_NAME=_matrix

.PHONY: clean all

all: _matrix.cpp
	g++ -O3 -Wall -shared -std=c++17 -fPIC ${PY_INCLUDE} ${SO_NAME}.cpp -o ${SO_NAME}${PY_CFG} ${MKL_PATH}

test: all
	python3 -m pytest -v 

clean:
	rm -f *.so
	rm -rf __pycache__
	rm -rf .pytest_cache

