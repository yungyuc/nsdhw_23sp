

.PHONY: all clean test

all: _vector.so

clean:
	rm -f *.so

test:
	python3 -m pytest angle_test.py -v


_vector.so: main.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) main.cpp $(shell python3-config --includes --ldflags) -o _vector.so
