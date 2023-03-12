CC = g++

.PHONY: all
all: ./q1_output

.PHONY: run
run:
		./q1_output

.PHONY: clean
clean:
		rm -f ./q1_output result.txt

.PHONY: check
		./q1_output > result.txt

./q1_output: Line.cpp Line.hpp main.cpp
		$(CC) Line.cpp Line.hpp main.cpp -o q1_output