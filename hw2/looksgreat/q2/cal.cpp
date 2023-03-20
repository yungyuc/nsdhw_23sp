#include<cmath>
#include<vector>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

using namespace std;
double cal_angle(double v1x, double v1y, double v2x, double v2y){
    double dot_prod = v1x*v2x + v1y*v2y;
    double mag1 = sqrt(v1x*v1x + v1y*v1y), mag2 = sqrt(v2x*v2x + v2y*v2y);
    if(mag1 == 0 || mag2 == 0) return 0;
    double cosine = dot_prod / (mag1 * mag2);
    if(cosine > 1) cosine = 1;
    else if(cosine < -1) cosine = -1;
    return acos(cosine);
}

PYBIND11_MODULE(_vector, m) {
    m.def("cal_angle_in_python", &cal_angle, "Calculate angle");
}