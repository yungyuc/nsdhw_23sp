CXX = g++
CFLAGS = -g -Wall

all: q1

q1: q1.cpp
	$(CXX) $(CFLAGS) -o q1 q1.cpp

run:
	./q1

check:
	./q1 > result.txt

clean:
	rm q1 result.txt > /dev/null 2>&1 || true
