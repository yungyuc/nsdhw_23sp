q1_result: line.o main.o
	g++ line.o main.o -o q1_result

line.o: Line.cpp Line.hpp
	g++ -c Line.cpp -o line.o

main.o: main.cpp Line.hpp
	g++ -c main.cpp -o main.o

.PHONY: clean
clean:
	rm -rf q1_result *.o *.txt

.PHONY: run
run:
	./q1_result

.PHONY: check
check:
	./q1_result > result.txt