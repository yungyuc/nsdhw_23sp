.PHONY: all test clean

CC = g++
CFLAGS = -g -Wall -O3 -Wall -shared -std=c++11 -fPIC

SRC = _vector.cpp
TARGET = $(patsubst %.cpp, %.so, $(SRC))

all: $(TARGET)
	
test: $(TARGET)
	python3 -m pytest -v

clean:
	rm -r *.so __pycache__/ .pytest_cache/

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET) `python3-config --includes --ldflags`