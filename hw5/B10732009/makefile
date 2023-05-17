.PHONY: all clean

CC = clang++
CFLAGS = -O3 -Wall -m64 -shared -std=c++17 -fPIC
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIB_SO=libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MKL_LIBS=$(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) \
			-lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

NUMPY= $(shell python3 -c "import numpy; print(numpy.get_include())")

SRC =  ../mod.cpp matrix.cpp
TARGET = _matrix.so

all: $(TARGET)

test: $(TARGET)
	python3 -m pytest -v

clean:
	rm -rf *.o *.out *.so __pycache__/ .pytest_cache/

$(TARGET): $(SRC)
	$(CC) $(CFLAGS)  -I./ -I/usr/include/mkl -I$(NUMPY) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET) `python3-config --includes --ldflags` ${MKL_LIBS}