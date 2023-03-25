CXX=g++

ifeq ($(shell uname),Darwin) # MacOS
	CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC -arch arm64 -undefined dynamic_lookup `python3-config --cflags --ldflags` 
else ifeq ($(shell uname),Linux) # Linux
	CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags` 
else
	echo "Don't know which OS system"
endif

SOURCES = _matrix.cpp
LIBRARY = _matrix.so

.PHONY: test clean

all: $(LIBRARY)

$(LIBRARY): $(SOURCES)
	$(CXX) $(CXXFLAGS) -O3 -o $@ $^

test: $(LIBRARY)
	env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v -p no:cacheprovider
	# python performance.py
	python -m pytest ../validate.py

clean:
	rm -rf __pycache__
	rm -f *.o *.so
    


