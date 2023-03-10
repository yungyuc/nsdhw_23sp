#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

double _angle(std::vector<double> v1, std::vector<double> v2)
{
    if (v1.size() != 2 || v2.size() != 2)
        return -1.0f;
    double len1 = std::sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    double len2 = std::sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    if (len1 < 1e-6 || len2 < 1e-6)
        return -1.0;
    double dot = v1[0] * v2[0] + v1[1] * v2[1];
    return std::acos(dot / len1 / len2);
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "Calculate the angle between 2 vectors";
    m.def("_angle", &_angle, "A function which calculates the angle between 2 vectors (in radians)");
}