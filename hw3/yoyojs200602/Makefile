GXX = g++
FLAGS = -O3 -m64 -shared -std=c++17 -fPIC
# MKL_LIB_DIR = /usr/lib/x86_64-linux-gnu
MKL_LIB_DIR = /home/stone/intel/mkl/lib/intel64
MKL_LIB_SO = libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MK_INC = /home/stone/intel/mkl/include

# MKL_LIBS = $(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) -lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5
MKL_LIBS = $(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) -lpthread -lm -ldl -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5
SRC = _matrix.cpp
TAR = $(patsubst %.cpp, %.so, $(SRC))

test: $(TAR)
	python3 -m pytest -v

clean:
	rm -rf *.o *.so __pycache__ .pytest_cache

%.so: %.cpp
	$(GXX) $(FLAGS) -I$(MK_INC) `python3 -m pybind11 --includes` $< -o $@ `python3-config --includes --ldflags` ${MKL_LIBS}
