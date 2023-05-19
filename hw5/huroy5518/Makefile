SO = _matrix.so

BUILD_DIR=build

PYBIND_INCLUDES := `python3 -m pybind11 --includes`
PYTHON_INCLUDES :=  `python3-config --includes --ldflags`

.PHONY: all clean test

all: _matrix.so

clean:
	rm -f *.so
	rm -rf build/

test: $(SO)
	python3 -m pytest -v


$(SO): ../mod.cpp
	g++ -O3 -Wall -shared -std=c++17 -fPIC $(PYBIND_INCLUDES) $< $(PYTHON_INCLUDES) -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -I/usr/include/mkl -I. -o $@

$(shell mkdir -p build/)