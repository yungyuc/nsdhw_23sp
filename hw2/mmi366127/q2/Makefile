PY_INC=`python3 -m pybind11 --includes`
PY_CFG=`python3-config --extension-suffix` `python3-config --includes --ldflags`
SO_NAME=angle

.PHONY: clean all

all: _vector

clean:
	rm -f *.so
	rm -r __pycache__

_vector: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC ${PY_INC} _vector.cpp -o _vector${PY_CFG}

test: all
	pytest -v
