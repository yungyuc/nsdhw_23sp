CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3-config --includes` 
CIMPORTPATH = includes/
LDFLAGS= -lblas

all: _matrix.so

_matrix.so: srcs/matrix.cpp srcs/TileHelper.cpp matrix_mul.cpp
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -I $(CIMPORTPATH) -o $@ ${LDFLAGS}

clean:
	rm -f *.so

test: _matrix.so
	python3 -m pytest test_matrix_mul.py -v

performance: _matrix.so
	python3 performance.py
	
.PHONY: all clean test