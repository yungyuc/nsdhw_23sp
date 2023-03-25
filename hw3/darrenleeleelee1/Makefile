CXXPROC = g++
ifeq ($(shell uname),Darwin)
  	# MacOS
	# MacOS need `-undefined dynamic_lookup`
	CPPFLAGS = -O3 -Wall -shared -std=c++11 -I./inc -fPIC `python3 -m pybind11 --includes` -undefined dynamic_lookup `python3-config --includes --ldflags`
else ifeq ($(shell uname),Linux)
	# Linux
	CPPFLAGS = -O3 -Wall -shared -std=c++11 -I./inc -fPIC `python3-config --includes`
else
  	echo "Don't know the OS"
endif
LFLAGS := -lblas
LIB_TARGET = _matrix.so


all: $(LIB_TARGET) $(PROG_TARGET)

$(LIB_TARGET): ./src/_matrix.cpp ./inc/matrix.hpp ./inc/matrix_multiply.hpp
	$(CXXPROC) -o $@ $(CPPFLAGS) $(LFLAGS) $<

.PHONY: test
test: $(LIB_TARGET)
	python3 -m pytest -v -s

.PHONY: clean
clean:
	rm -f *.o *.so $(LIB_TARGET) $(PROG_TARGET)