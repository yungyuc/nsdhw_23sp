SOURCE = ./mod.cpp
SO = _matrix$(shell python3-config --extension-suffix)

all: $(SO)

$(SO): $(SOURCE) matrix.hpp
	g++ -O3 -Wall -std=c++17 -shared -fPIC $(SOURCE) -o $(SO) \
		-lblas \
		$(shell python3 -m pybind11 --includes) $(shell python3-config --includes --ldflags) \

clean:
	rm -f $(SO)
	rm -rf __pycache__
