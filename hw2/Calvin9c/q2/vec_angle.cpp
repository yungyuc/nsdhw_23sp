#include<iostream>
#include<cmath>

#include<pybind11/pybind11.h>

using namespace std;
namespace py = pybind11;

double vec_angle(double u_x, double u_y, double v_x, double v_y){
    double u_length = sqrt( u_x*u_x + v_y*v_y );
    double v_length = sqrt( v_x*v_x + v_y*v_y );

    if ( u_length==0.0 or v_length==0.0 ) {
        return NAN;
    }
    double dot_product = u_x*v_x + u_y*v_y;

    double theta = acos(dot_product/(u_length*v_length));

    return theta;
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "Convert c++ program to python one.";
    m.def(
        "vec_angle",
        &vec_angle, 
        "return the angle of two 2D-vector."
    );
}