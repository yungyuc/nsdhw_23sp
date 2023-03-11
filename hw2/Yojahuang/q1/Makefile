CXX = g++
CFLAG = -std=c++11

# Build and run the program
run: *.cpp
	$(CXX) *.cpp $(CFLAG) && ./a.out

# Redirects the terminal output to a file result.txt
check: *.cpp
	$(CXX) *.cpp $(CFLAG) && ./a.out > result.txt

# Remove all the built and intermediate files
clean: 
	rm -rf *.o *.out *.txt
