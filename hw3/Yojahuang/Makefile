CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3-config --includes` 
CIMPORTPATH = includes/
MKLROOT=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKLROOT}/libmkl_def.so \
		${MKLROOT}/libmkl_avx2.so \
		${MKLROOT}/libmkl_core.so \
		${MKLROOT}/libmkl_intel_lp64.so \
		${MKLROOT}/libmkl_sequential.so \
		-lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

all: _matrix.so

_matrix.so: srcs/matrix.cpp srcs/TileHelper.cpp matrix_mul.cpp
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -I $(CIMPORTPATH) -o $@ ${LDFLAGS}

clean:
	rm -f *.so

test: _matrix.so
	python3 -m pytest test_matrix_mul.py -v
	
.PHONY: all clean test