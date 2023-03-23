CXX := g++
CXXFLAGS := -O3 -std=c++11 -fPIC -Wall -Wextra -pedantic -I include
LFLAGS := -lpthread -lm -ldl -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

MKL_LIB_DIR := /usr/lib/x86_64-linux-gnu
MKL_LIBS := $(MKL_LIB_DIR)/libmkl_def.so
MKL_LIBS := $(MKL_LIBS) $(MKL_LIB_DIR)/libmkl_avx2.so
MKL_LIBS := $(MKL_LIBS) $(MKL_LIB_DIR)/libmkl_core.so
MKL_LIBS := $(MKL_LIBS) $(MKL_LIB_DIR)/libmkl_intel_lp64.so
MKL_LIBS := $(MKL_LIBS) $(MKL_LIB_DIR)/libmkl_sequential.so

PYTHON_INCLUDES := `python3-config --includes`

LIB := _matrix.so

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,bin/%.o,$(SRCS))

DIR_GUARD=@mkdir -p $(@D)

all: $(LIB)

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS) $(MKL_LIBS) $(LFLAGS)

bin/%.o: src/%.cpp
	$(DIR_GUARD)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) -o $@ -c $<

test: $(LIB)
	python3 -m pytest -v -s

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all test clean
