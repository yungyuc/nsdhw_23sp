CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags`
SO = _matrix.so

.PHONY: test clean

all: $(SO)

_matrix.so: matrix.cpp
	$(CXX) $(CFLAGS) -o $@ $<

test: $(SO)
	python3 -m pytest -v -s

clean:
	rm -f *.so
	rm -rf __pycache__ .pytest_cache