
PROGRAM_NAME=Line

.PHONY: clean all

all: ${PROGRAM_NAME}

run: ${PROGRAM_NAME}
	./${PROGRAM_NAME}

check: ${PROGRAM_NAME}
	./${PROGRAM_NAME} > result.txt

clean: 
	rm -f Line

${PROGRAM_NAME}: Line.cpp
	g++ -O3 -Wall -std=c++11 Line.cpp -o ${PROGRAM_NAME}
