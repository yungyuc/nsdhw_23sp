CXX = g++
CXXFLAGS = -std=c++11 -O3 -g -Wall
EXEC = line

all: line.cpp
	$(CXX)  $(CXXFLAGS) -o line $<

run: $(EXEC)
	./$(EXEC)

check: $(EXEC)
	./$(EXEC) > result.txt

clean:
	rm -rf *.o $(EXEC) result.txt