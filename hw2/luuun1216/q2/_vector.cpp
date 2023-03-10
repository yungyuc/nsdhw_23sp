# include <iostream>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>
# include <cmath>

using namespace std;
namespace py = pybind11;

float calculate_vector_angle(float x1, float y1, float x2, float y2){
    if(((x1==0) && (y1==0)) || ((x2==0) && (y2==0)))
    {
        throw invalid_argument("zero-length 2-vector");
    }
    float angle = atan2(y2, x2) - atan2(y1, x1);;
    return angle * 180 / 3.141592;
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 which is to change the c++ program to python.";
  m.def("calculate_vector_angle",
         &calculate_vector_angle,
        "calculate angle between two vector."
       );
}