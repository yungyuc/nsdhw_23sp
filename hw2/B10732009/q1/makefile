.PHONY: all run check clean

CC = g++
CFLAGS = -g -Wall

SRC = main.cpp line.cpp
OBJ = $(patsubst %.cpp, %.o, $(SRC))
TARGET = main.out

all: $(TARGET)

run:
	./$(TARGET)

check:
	./$(TARGET) > result.txt
clean:
	rm *.o *.out *.txt

$(TARGET): $(OBJ) 
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<