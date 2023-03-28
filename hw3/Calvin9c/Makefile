CC = g++
CCFlags = -O3 -Wall -shared -std=c++11 -fPIC

Headers = -I/usr/include/mkl

PyBindFlags = `python3 -m pybind11 --includes` `python3-config --includes --ldflags`

MKL_LIB_DIR = /usr/lib/x86_64-linux-gnu
MKL_LIB_SO = libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MKL_LIBS = $(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) \
			-lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

SRC = _mat.cpp
OBJ = _mat.so

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $< -o $@ $(CCFlags) $(Headers) $(PyBindFlags) ${MKL_LIBS}

test: $(OBJ)
	python3 -m pytest -v -s

clean:
	rm *.o *.so __pycache__ .pytest_cache performance.txt -r -f