#include <pybind11/pybind11.h>

#include <cmath>
#include <utility>

namespace py = pybind11;

double angle_between_vectors(std::pair<double, double> u, std::pair<double, double> v) {
    double inner_product = (u.first * v.first) + (u.second * v.second);
    double magnitude = sqrt((u.first * u.first) + (u.second * u.second)) 
                     * sqrt((v.first * v.first) + (v.second * v.second));
    
    return acos(inner_product/magnitude);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("angle_between_vectors", &angle_between_vectors, 
            "A function that calculates the angle (in radians) between two \
            vectors in the 2-dimensional Cartesian coordinate system.");
}
