CXX = g++
FLAGS = -O3 -Wall -std=c++17
INCLUDES = $(shell pkg-config --cflags mkl-dynamic-lp64-iomp) $(shell python3-config --includes) $(shell python3 -m pybind11 --includes) $(shell python3 -c "import numpy; print('-I%s'%numpy.get_include())")
LINK = $(shell pkg-config --libs mkl-dynamic-lp64-iomp)
SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _matrix$(SUFFIX)

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=$(MKL_LIB_DIR)/libmkl_def.so
MKL_LIBS=$(MKL_LIBS):$(MKL_LIB_DIR)/libmkl_avx2.so
MKL_LIBS=$(MKL_LIBS):$(MKL_LIB_DIR)/libmkl_core.so
MKL_LIBS=$(MKL_LIBS):$(MKL_LIB_DIR)/libmkl_intel_lp64.so
MKL_LIBS=$(MKL_LIBS):$(MKL_LIB_DIR)/libmkl_sequential.so

$(TARGET): matrix.o mod.o
	$(CXX) $(FLAGS) -shared $^ -o $@ $(LINK)

%.o: %.cpp matrix.hpp
	$(CXX) $(FLAGS) $(INCLUDES) -fPIC -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) performance.txt __pycache__/ .pytest_cache/ *.o

test: $(TARGET)
	export LD_PRELOAD=$$MKL_LIBS
	python3 -m pytest -v
