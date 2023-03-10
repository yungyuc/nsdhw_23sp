CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
SRC = myLine.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = program

.PHONY: clean run check

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) result.txt

run: $(TARGET)
	./$(TARGET)

check: run
	./$(TARGET) > result.txt

