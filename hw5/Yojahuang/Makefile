CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3-config --includes` 
LDFLAGS= -lblas

all: _matrix.so

_matrix.so: matrix_mul.cpp ../mod.cpp
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -o $@ ${LDFLAGS}

clean:
	rm -f *.so

.PHONY: all clean
