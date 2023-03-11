//dot_product = a.x * b.x + a.y * b.y
//angle = acos(dot_product / (|a| * |b|))

#include <cmath>
#include <pybind11/pybind11.h>
#include<vector>
#include <pybind11/stl.h> //change python list to C++ vector<T>

double cal_angle(std::vector<double>& coor_1, std::vector<double>& coor_2){
    double dot_product = coor_1[0] * coor_2[0] + coor_1[1] * coor_2[1];
    double length_1 = sqrt(pow(coor_1[0], 2) + pow(coor_1[1], 2));
    double length_2 = sqrt(pow(coor_2[0], 2) + pow(coor_2[1], 2));
    if(!length_1)
        throw std::invalid_argument("The length of vector 1 is zero.");
    else if(!length_2)
        throw std::invalid_argument("The length of vector 2 is zero.");
    double angle = acos(dot_product / (length_1 * length_2));

    return angle;
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 wrapping";      // module doc string
  m.def("cal_angle",                              // function name
        &cal_angle,                               // function pointer
        "A function which calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system" //function doc string
       );
}