CXXPROC = clang++
ifeq ($(shell uname),Darwin)
  	# MacOS
	# MacOS need `-undefined dynamic_lookup`
	CPPFLAGS = -O3 -Wall -shared -std=c++11 -I./inc -fPIC `python3 -m pybind11 --includes` -undefined dynamic_lookup `python3-config --includes --ldflags`
else ifeq ($(shell uname),Linux)
	# Linux
	CPPFLAGS = -O3 -Wall -m64 -shared -std=c++11 -fPIC -I./inc -fPIC `python3-config --includes`
else
  	echo "Don't know the OS"
endif
LIB_TARGET = _matrix.so
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIB_SO=libmkl_def.so libmkl_avx2.so libmkl_core.so libmkl_intel_lp64.so libmkl_sequential.so
MKL_LIBS=$(patsubst %, $(MKL_LIB_DIR)/%, $(MKL_LIB_SO)) \
   -lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

all: $(LIB_TARGET) $(PROG_TARGET)

$(LIB_TARGET): ./src/_matrix.cpp ./inc/matrix.hpp ./inc/matrix_multiply.hpp
	$(CXXPROC) $(CFLAGS) -I/usr/include/mkl python3 -m pybind11 --includes $< -o $@ python3-config --includes --ldflags ${MKL_LIBS}
.PHONY: test
test: $(LIB_TARGET)
	python3 -m pytest -v -s

.PHONY: clean
clean:
	rm -f *.o *.so $(LIB_TARGET) $(PROG_TARGET)