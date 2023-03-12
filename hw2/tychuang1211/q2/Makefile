CXX = g++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDES = $(shell python3-config --includes) $(shell python3 -m pybind11 --includes)
SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _vector$(SUFFIX)

$(TARGET): libvector.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $^ -o $(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) __pycache__/ .pytest_cache/

.PHONY: test
test:
	pytest
