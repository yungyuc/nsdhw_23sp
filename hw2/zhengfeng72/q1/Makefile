CC	   = g++
CFLAGS = -g -Wall -Werror -std=c++11
EXE    = main.o

.PHONY: run check clean

main: main.o line.o
	$(CC) $^ -o $@ $(CFLAGS)

main.o:main.cpp line.hpp
	$(CC) -c $< -o $@ $(CFLAGS)

line.o:line.cpp line.hpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm *.o result.txt main

run:
	./main

check:
	./main > result.txt