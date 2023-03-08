.PHONY: all test clean

CC = g++
CFLAGS = -g -Wall -O3 -Wall -shared -std=c++11 -fPIC
INC = /usr/include/python3.8

SRC = _vector.cpp
TARGET = $(patsubst %.cpp, %, $(SRC))
FULL_TARGET = $(TARGET)*.so

all: $(FULL_TARGET)
	
test: $(FULL_TARGET)
	python3 -m pytest -v

clean:
	rm -r *.so __pycache__/ .pytest_cache/

$(FULL_TARGET): $(SRC)
	$(CC) $(CFLAGS) -I $(INC) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET)`python3-config --extension-suffix`