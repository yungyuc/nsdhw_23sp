CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -shared
HEADERS = -I.# -I/DATA/lllll/.local/lib/python3.6/site-packages/pybind11/include/pybind11/
PIBINDFLAG = -fPIC `python3 -m pybind11 --includes` `python3-config --includes --ldflags`
PY_OBJ = _vector.so

all: $(PY_OBJ)

$(PY_OBJ): cal.cpp
	$(CXX)  $(CXXFLAGS) $^  $(HEADERS) $(PIBINDFLAG)  -o $@

test: all
	python3 -m pytest -v

clean:
	rm *.o *.so -r -f