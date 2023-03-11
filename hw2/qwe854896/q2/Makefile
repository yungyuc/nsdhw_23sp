CC := g++
CFLAGS := -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`

TARGET := _vector.so

all: $(TARGET) 

$(TARGET): _vector.cpp
	$(CC) $(CFLAGS) _vector.cpp -o $(TARGET)

test:
	python3 -m pytest -v

clean:
	rm -rf $(TARGET)

.PHONY: all test clean