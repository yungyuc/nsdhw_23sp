CXXPROC = g++
CPPFLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags` # for debug
PROG_TARGET = angle_test.py
LIB_TARGET = _vector.so

all: $(LIB_TARGET)
$(LIB_TARGET): angle.cpp
	$(CXXPROC) -o $@ $(CPPFLAGS) $<

.PHONY: test
test:
	env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v

.PHONY: clean
clean:
	rm -rf *.o *.so .pytest_cache/ __pycache__/