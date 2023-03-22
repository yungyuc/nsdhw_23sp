CXX=clang++
CXXFLAGS=-g -O3 -Wall -m64 -shared -std=c++11 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`

MKLROOT=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKLROOT}/libmkl_def.so \
		${MKLROOT}/libmkl_avx2.so \
		${MKLROOT}/libmkl_core.so \
		${MKLROOT}/libmkl_intel_lp64.so \
		${MKLROOT}/libmkl_sequential.so \
		-lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5
all: Matrix.h _matrix.cpp
	${CXX} $(CXXFLAGS) -I/usr/include/mkl $(PYBIND) $(shell python3-config --includes --ldflags) _matrix.cpp -o _matrix$(PYCONFIG) -I/usr/include/python3.8 ${LDFLAGS}

test: all
	python3 -m pytest -v -s

perf: all
	python3 perf.py

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt
