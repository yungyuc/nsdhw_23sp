CXXPROC = g++
CPPFLAGS = -std=c++17 -O3 -Wall
# CPPFLAGS = -std=c++17  -g -O0 -Wall # for debug
PROG_TARGET = prog


all: $(PROG_TARGET)
$(PROG_TARGET): line.cpp
	$(CXXPROC) -o $@ $(CPPFLAGS) $<

.PHONY: run
run:
	./$(PROG_TARGET)

.PHONY: check
check:
	./$(PROG_TARGET) > result.txt

.PHONY: clean
clean:
	rm -f $(PROG_TARGET) *.o *.so