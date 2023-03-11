CXX = g++
CFLAGS = -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags`

.PHONY: test clean

all: _vector.so

_vector.so: cal_angle.cpp
	$(CXX) $(CFLAGS) -o $@ $<

test:
	python3 -m pytest ./cal_angle_test.py -v

clean:
	rm -f *.so *.o