CXXFLAGS=-O3 -Wall -m64 -shared -std=c++11 -fPIC
pybind_include=`python3 -m pybind11 --includes`
python_config=`python3-config --extension-suffix`
src=_matrix

.PHONY: all test clean

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so \
		-lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

all:
	clang++ $(CXXFLAGS) -I/usr/include/mkl $(pybind_include) $(src).cpp -o $(src)$(python_config) -I/usr/include/python3.10 ${MKL_LIBS} 

test: all
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache 