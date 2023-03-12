CXX = g++
TARGET = main
OUTPUT = result.txt

$(TARGET): main.o
	$(CXX) $^ -o $(TARGET)

main.o: main.cpp line.hpp
	$(CXX) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OUTPUT) *.o

.PHONY: run
run:
	./$(TARGET)

.PHONY: check
check:
	./$(TARGET) > $(OUTPUT)