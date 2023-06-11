# Variables
BUILD_DIR = build
SO = _matrix.so
CPP = ../mod.cpp
PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes)
PYTHON_INCLUDES := $(shell python3-config --includes --ldflags)
CXXFLAGS := -O3 -Wall -shared -std=c++17 -fPIC $(PYBIND_INCLUDES) $(PYTHON_INCLUDES) -I/usr/include/mkl -I.
LDFLAGS := -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

# Default target
.PHONY: all
all: $(SO)

# Build shared object file
$(SO): $(CPP)
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	g++ $(CXXFLAGS) $< $(LDFLAGS) -o $@

# Test target
.PHONY: test
test: $(SO)
	python3 -m pytest -v

# Clean target
.PHONY: clean
clean:
	rm -f *.so
	rm -rf $(BUILD_DIR)