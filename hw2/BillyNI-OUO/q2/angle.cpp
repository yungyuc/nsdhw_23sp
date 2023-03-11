#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>

using namespace std;

double cal_angle(double v1_x, double v1_y, double v2_x, double v2_y){
    double dot_prod = v1_x*v2_x + v1_y*v2_y;
    double mag_1 = sqrt(v1_x*v1_x + v1_y*v1_y);
    double mag_2 = sqrt(v2_x*v2_x + v2_y*v2_y);
    if(mag_1 == 0 || mag_2 == 0){
    	throw std::runtime_error("One or both input vectors have zero length");
    }
    double cosin = dot_prod / (mag_1 * mag_2);
    
    if(cosin > 1){
    	acos(1);
    }else if(cosin < -1){
    	acos(-1);	
    } 
    return acos(cosin);
}

PYBIND11_MODULE(_angle, m) {
    m.def("cal_angle_in_python", &cal_angle, "Calculate angle in 2 vector");
}