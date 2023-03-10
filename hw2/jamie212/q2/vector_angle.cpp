#include <pybind11/pybind11.h>
#include <cmath>

namespace py = pybind11;

double angleBetweenVectors(double x1, double y1, double x2, double y2) {
    double dotProduct = x1 * x2 + y1 * y2;
    double magnitude1 = sqrt(x1 * x1 + y1 * y1);
    double magnitude2 = sqrt(x2 * x2 + y2 * y2);
    if (magnitude1 == 0 || magnitude2 == 0) {
        return NAN;  // return NaN if either vector has zero length
    }
    double cosTheta = dotProduct / (magnitude1 * magnitude2);
    return acos(cosTheta);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("angleBetweenVectors", &angleBetweenVectors, "A function that calculates the angle between two vectors in the 2D Cartesian coordinate system");
}
