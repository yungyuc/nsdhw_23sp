#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

double angle_between_vectors(const std::vector<double>& v1, const std::vector<double>& v2) {
    double dot_product = v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1);
    double mag_v1 = std::sqrt(v1.at(0) * v1.at(0) + v1.at(1) * v1.at(1));
    double mag_v2 = std::sqrt(v2.at(0) * v2.at(0) + v2.at(1) * v2.at(1));
    if(mag_v1 == 0 || mag_v2 == 0){
        throw std::runtime_error("One or both input vectors have zero length");
    }
    double cos_theta = dot_product / (mag_v1 * mag_v2);
    return std::acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
    m.def("angle_between_vectors", &angle_between_vectors, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system");
}
