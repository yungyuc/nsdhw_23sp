CXX = g++
CFLAGS = -std=c++11 -g -Wall
EXE = q1

.PHONY: run check clean

all: $(EXE)

$(EXE): main.o Line.o
	$(CXX) $(CFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

Line.o: Line.cpp Line.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

run:
	./$(EXE)

check:
	./$(EXE) 1>result.txt

clean:
	rm -f $(EXE) *.o result.txt