FLAG = -shared -O3 -std=c++11 -fPIC
PYBINDFLAG = `python3 -m pybind11 --includes`
PYCONFFLAG = `python3-config --extension-suffix` `python3-config --ldflags`
GXX = g++
OBJ = _vector

all: _vector.*.so

_vector.*.so: q2.cpp
	$(GXX) $(FLAG) $(PYBINDFLAG) q2.cpp -o $(OBJ)$(PYCONFFLAG)

.PHONY: test
test: all
	python3 -m pytest -v

.PHONY: clean
clean:
	rm *.so 