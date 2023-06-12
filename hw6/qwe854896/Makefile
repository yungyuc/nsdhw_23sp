CXX := g++
CXXFLAGS := -O3 -std=c++14 -fPIC -Wall -Wextra -pedantic -I include
LFLAGS := -lblas

PYTHON_INCLUDES := `python3-config --includes`

LIB := _matrix.so

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,bin/%.o,$(notdir $(SRCS)))

DIR_GUARD=@mkdir -p $(@D)

all: $(LIB)

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS) $(LFLAGS)

bin/%.o: ../%.cpp
	$(DIR_GUARD)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) -o $@ -c $<

bin/%.o: src/%.cpp
	$(DIR_GUARD)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) -o $@ -c $<

test: $(LIB)
	python3 -m pytest -v -s

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all test clean
