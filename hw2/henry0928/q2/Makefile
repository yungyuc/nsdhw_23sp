CC = g++
CFLAGS = -g -Wall -O3 -Wall -shared -std=c++11 -fPIC

SRC = _vector.cpp
TARGET = $(patsubst %.cpp, %.so, $(SRC))

build: _vector.cpp
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET) `python3-config --includes --ldflags`
test: test_.py
	python3 -m pytest -v
clean:
	rm -f _vector.so
	rm -r -f __pycache__
