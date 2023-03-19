CXX := g++
CXXFLAGS := -O3 -std=c++11 -fPIC -Wall -Wextra -pedantic -I include

PYTHON_INCLUDES := `python3-config --includes`

LIB := _matrix.so

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,bin/%.o,$(SRCS))

DIR_GUARD=@mkdir -p $(@D)

all: $(LIB)

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS)

bin/%.o: src/%.cpp
	$(DIR_GUARD)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) -o $@ -c $<

test: $(LIB)
	python3 -m pytest -v

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all test clean
