CXX = g++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDES = $(shell pkg-config --cflags mkl-dynamic-lp64-iomp) $(shell python3-config --includes) $(shell python3 -m pybind11 --includes)
LINK = $(shell pkg-config --libs mkl-dynamic-lp64-iomp)
SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _matrix$(SUFFIX)

$(TARGET): matrix.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $^ -o $@ $(LINK)

.PHONY: clean
clean:
	rm -rf $(TARGET) performance.txt __pycache__/ .pytest_cache/

test: $(TARGET)
	python3 -m pytest -v
