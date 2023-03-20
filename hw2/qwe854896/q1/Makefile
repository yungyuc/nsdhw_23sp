CC := g++
CFLAGS := -Wall -g

TARGET := main
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,bin/%.o,$(SRCS))

DIR_GUARD=@mkdir -p $(@D)

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

bin/%.o: %.cpp Line.h
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ -c $<

run:
	./main

check:
	./main > result.txt

clean:
	rm -rf $(TARGET) bin result.txt

.PHONY: all run check clean