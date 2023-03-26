GXX = g++
FLAGS = -O3 -m64 -shared -std=c++14 -fPIC

MKL_ROOT = /usr/lib/x86_64-linux-gnu
MKL_INC = /usr/include/mkl
MKL_LIB_SO = libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MKL_LD_INFO = $(patsubst %, $(MKL_ROOT)/%, $(MKL_LIB_SO)) -lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5  -L/opt/intel/lib/intel64 -L/opt/intel/mkl/lib/intel64 

# MKL_LD_INFO = $(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) -lpthread -lm -ldl -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5
# MKL_LIB_DIR = /home/stone/intel/mkl/lib/intel64
# MKL_INC = /home/stone/intel/mkl/include


SRC = _matrix.cpp
TAR = _matrix.so

all: $(TAR)

test: $(TAR)
	python3 -m pytest -vv -s

$(TAR): $(SRC)
	$(GXX) $(FLAGS) -I$(MKL_INC) `python3 -m pybind11 --includes` $< -o $@ `python3-config --includes --ldflags` ${MKL_LD_INFO}

.PHONY: clean
clean:
	rm -rf *.o *.so __pycache__ .pytest_cache
