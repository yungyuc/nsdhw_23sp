CXXPROC = g++
CPPFLAGS = -O3 -Wall -shared -std=c++11 -fPIC  `python3 -m pybind11 --includes` `python3-config --includes --ldflags` # for debug
PROG_TARGET = matrix_test.py
LIB_TARGET = _matrix.so
MKLROOT=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKLROOT}/libmkl_def.so \
		${MKLROOT}/libmkl_avx2.so \
		${MKLROOT}/libmkl_core.so \
		${MKLROOT}/libmkl_intel_lp64.so \
		${MKLROOT}/libmkl_sequential.so \
		-lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

all: $(LIB_TARGET)
$(LIB_TARGET): matrix.cpp
	$(CXXPROC) -o $@ $(CPPFLAGS) ${LDFLAGS} $<

.PHONY: test
test:
	env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v

.PHONY: clean
clean:
	rm -rf *.o *.so .pytest_cache/ __pycache__/