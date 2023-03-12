CC = g++

include_pybind = `python3 -m pybind11 --includes`
CCFlags = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` vec_angle.cpp `python3-config --includes --ldflags` -o _vector.so

OBJ = _vector.so
SRC = vec_angle.cpp

.PHONY: all
all: $(OBJ)

.PHONY: clean
clean:
	rm -f *.so

.PHONY: test
test:
		python3 -m pytest test_vec_angle.py -v

_vector.so: $(SRC)
		$(CC) $(CCFlags)