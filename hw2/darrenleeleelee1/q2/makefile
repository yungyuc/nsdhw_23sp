CXXPROC = g++
ifeq ($(shell uname),Darwin)
  	# MacOS
	# MacOS need `-undefined dynamic_lookup`
	CPPFLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` -undefined dynamic_lookup `python3-config --includes --ldflags` # for debug
else ifeq ($(shell uname),Linux)
	# Linux
	CPPFLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags` # for debug
else
  	echo "Don't know the OS"
endif
PROG_TARGET = test_angle.py
LIB_TARGET = _vector.so

all: $(LIB_TARGET)
$(LIB_TARGET): angle.cpp
	$(CXXPROC) -o $@ $(CPPFLAGS) $<

.PHONY: test
test:
	pytest ./$(PROG_TARGET)

.PHONY: clean
clean:
	rm -f *.o *.so
