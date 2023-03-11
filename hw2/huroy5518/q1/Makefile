

.PHONY: all clean run check

all: ./main

run:
	./main

clean:
	rm -f ./main result.txt

check:
	./main > result.txt

./main: line.cpp line.hpp main.cpp
	g++ line.cpp line.hpp main.cpp -o main
