CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags` 

all: _vector.so

_vector.so: getAngle.cpp
	$(CXX) $(CFLAGS) getAngle.cpp -o _vector.so

.PHONY: test
test:
	python3 -m pytest getAngle_test.py -v

.PHONY: clean
clean:
	rm -f *.so *.o