.PHONY: all test benchmark clean

all:
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $(shell python3-config --includes --ldflags) _matrix.cpp -o _matrix$(shell python3-config --extension-suffix)

test: all
	python3 -m pytest -v

benchmark:

clean:
	rm *.so performance.txt -rf __pycache__
