all:
	c++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $(shell python3-config --includes --ldflags) _vector.cpp -o _vector$(shell python3-config --extension-suffix)

test:
	python3 -m pytest

clean:
	rm *.so -rf __pycache__
