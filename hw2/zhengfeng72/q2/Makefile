CC     = g++
CFLAGS = -Wall -std=c++11 -shared -fPIC
PY     = python3 -m pybind11 --includes
SUFFIX = python3-config --extension-suffix

.PHONY: clean test

all: _vector.*.so

_vector.*.so: _vector.cpp
	$(CC) $(CFLAGS) `$(PY)` $< -o _vector`$(SUFFIX)`

clean:
	rm -rf *.so ./__pycache__ ./.pytest_cache

test: all
	python3 -m pytest -v