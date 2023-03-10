NAME = _vector
SOURCE = $(NAME).cpp
SO = $(NAME)$(shell python3-config --extension-suffix)

all: $(SO)

$(SO): $(SOURCE)
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $(SOURCE) $(shell python3-config --includes --ldflags) -o $(SO)

test:
	python -m pytest
	
clean:
	rm -f $(SO)
	rm -rf __pycache__
