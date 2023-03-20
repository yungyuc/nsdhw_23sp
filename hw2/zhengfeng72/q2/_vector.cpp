#include <iostream>
#include <vector>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

double cal_vec_angle(std::vector<double> v1, std::vector<double> v2){
    if(v1.size()!= 2 || v2.size()!=2) return -1;

    double v1_len = sqrt(v1[0]*v1[0] + v1[1]*v1[1]);
    double v2_len = sqrt(v2[0]*v2[0] + v2[1]*v2[1]);
    if(v1_len==0 || v2_len==0) return -1;
    
    double dot = v1[0]*v2[0] + v1[1]*v2[1];
    double angle = acos(dot/sqrt(v1_len * v2_len));

    return angle;
} 


PYBIND11_MODULE(_vector, m){
    m.doc() = "calulate vector plugin";
    m.def("cal_vec_angle", &cal_vec_angle, "A function that calulate angle of two vector.");
}