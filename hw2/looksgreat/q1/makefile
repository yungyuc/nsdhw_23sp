CXX = g++
CXXFLAGS = -std=c++11
HEADERS = -I.
EXEC = line

line: line.hpp line.cpp
	$(CXX)  $(CXXFLAGS) $^  $(HEADERS)  -o $@

run: $(EXEC)
	./$(EXEC)

check: $(EXEC)
	./$(EXEC) > result.txt

clean:
	rm *.o $(EXEC) -r -f