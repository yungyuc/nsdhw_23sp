.PHONY: all clean test bin

CC = clang++
CFLAGS = -O3 -Wall -m64 -shared -std=c++11 -fPIC
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIB_SO=libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MKL_LIBS=$(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) \
   -lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

SRC = ./src/_matrix.cpp
BIN_TARGET = matrix
LIB_TARGET = _matrix.so
all: $(LIB_TARGET)

$(LIB_TARGET): ./src/_matrix.cpp
	$(CC) $(CFLAGS) -I./inc -I/usr/include/mkl `python3 -m pybind11 --includes` $< -o $@ `python3-config --includes --ldflags` ${MKL_LIBS}

bin: $(BIN_TARGET)

$(BIN_TARGET): ./src/matrix.cpp
	$(CC) $(CFLAGS) -I./inc -I/usr/include/mkl $< -o $@ ${MKL_LIBS}

test: $(LIB_TARGET)
	python3 -m pytest -v -s

clean:
	rm -rf *.o *.out *.so __pycache__/ .pytest_cache/
