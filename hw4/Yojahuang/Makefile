CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3-config --includes` 
LDFLAGS= -lblas
CIMPORTPATH = srcs/

all: _matrix.so

_matrix.so: matrix_mul.cpp srcs/counter_and_allocator.hpp srcs/matrix.hpp 
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -I $(CIMPORTPATH) -o $@ ${LDFLAGS}

clean:
	rm -f *.so

.PHONY: all clean