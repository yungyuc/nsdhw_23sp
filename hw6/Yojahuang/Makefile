CXX = g++
CFLAGS = -std=c++14 -O3 -Wall -shared -fPIC `python3-config --includes` 
LDFLAGS= -lblas
CIMPORTPATH = "includes"

all: _matrix.so

_matrix.so: srcs/Matrix.cpp srcs/Matrix_Mul.cpp srcs/Pybind.cpp includes/Matrix.h includes/Matrix_Mul.h
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -I${CIMPORTPATH} -o $@ ${LDFLAGS}

clean:
	rm -f *.so

.PHONY: all clean
