GXX = g++
FLAG = -g -std=c++11
TARGET = q1.cpp q1.hpp
EXC = out

all: q1

q1: $(TARGET)
	$(GXX) $(FLAG) $(TARGET) -o $(EXC)

.PHONY: run
run:
	./$(EXC)

.PHONY: check
check:
	./$(EXC) > result.txt

.PHONY: clean
clean:
	rm $(EXC) result.txt