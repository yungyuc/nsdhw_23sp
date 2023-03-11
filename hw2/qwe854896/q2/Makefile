CC := g++
CFLAGS := -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`
LFLAGS := `python3-config --ldflags`

TARGET := _vector.so

all: $(TARGET) 

$(TARGET): _vector.cpp
	$(CC) $(CFLAGS) _vector.cpp -o $(TARGET) $(LFLAGS)

test:
	python3 -m pytest -v

clean:
	rm -rf $(TARGET)

.PHONY: all test clean