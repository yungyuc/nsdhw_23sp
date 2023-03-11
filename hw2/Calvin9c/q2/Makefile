CC = g++

include_pybind = python3 -m pybind11 --includes
CCFlags = -O3 -Wall -shared -std=c++11 -fPIC 

OBJ = _vector.so
SRC = vec_angle.cpp


.PHONY: all
all:
		_vector.so

.PHONY: clean
clean:
		rm -f *.so

.PHONY: test
test:
		python3 -m pytest vec_angle.py -v

_vector.so: vec_angle.cpp
		$(CC) $(CCFlags) `$(include_pybind)` $(SRC) $(shell python3-config --includes --ldflags) -o $(OBJ)
