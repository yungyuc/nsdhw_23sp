CC = g++

pybind11_include = python3 -m pybind11 --includes
cflages = -shared -O3 -Wall -std=gnu++14 -g -fPIC
suffix = python3-config --extension-suffix
object_file=_vector

all: $(object_file).*.so

$(object_file).*.so: $(object_file).cpp
	$(CC) -o $(object_file)`$(suffix)` $(cflages) `$(pybind11_include)` $< -I /usr/include/python3.8

.PHONY: test.py
test: all
	python3 -m pytest -v

.PHONY: clean
clean:
	rm -rf *.so __pycache__  .pytest_cache