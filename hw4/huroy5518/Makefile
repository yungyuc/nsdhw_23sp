SO = _matrix.so

BUILD_DIR=build

.PHONY: all clean test

all: _matrix.so

clean:
	rm -f *.so
	rm -rf build/

test: $(SO)
	python3 -m pytest -v


$(SO): matrix.cpp 
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $^ $(shell python3-config --includes --ldflags) -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -I/usr/include/mkl -o $@

$(shell mkdir -p build/)