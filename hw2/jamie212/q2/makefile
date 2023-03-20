CXX = g++
ifeq ($(shell uname),Darwin) # MacOS
	CXXFLAGS = -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup `python3-config --cflags --ldflags` 
else ifeq ($(shell uname),Linux) # Linux
	CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags` 
else
  	echo "Don't know which OS system"
endif
# CXXFLAGS = -O3 -Wall -std=c++11
SOURCES = vector_angle.cpp
LIBRARY = _vector.so
# LDLIBS = $(shell python3-config --cflags --ldflags)
.PHONY: test clean

all: $(LIBRARY)

$(LIBRARY): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(LIBRARY)
	python3 -m pytest -v -p no:cacheprovider

clean:
	rm -rf __pycache__
	rm -f *.o *.so
	