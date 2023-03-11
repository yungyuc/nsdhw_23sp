CC = g++

.PHONY: all
all: main

.PHONY: run
run:
		main

.PHONY: clean
clean:
		rm -f main result.txt

.PHONY: check
		main > result.txt

main: Line.cpp Line.hpp main.cpp
		$(CC) Line.cpp Line.hpp main.cpp -o q1_output