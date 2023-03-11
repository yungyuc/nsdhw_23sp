#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <cmath>
using namespace std;
namespace py = pybind11;

double angle_between(const vector<double> &v1, const vector<double> &v2)
{
    if (v1.size() != 2 || v2.size() != 2)
    {
        throw std::invalid_argument("vector is not in the 2-dimensional.");
    }
    if (((v1[0] == 0.0) && (v1[1] == 0.0)) || ((v2[0] == 0.0) && (v2[1] == 0.0)))
    {
        throw std::invalid_argument("vector can't be zero length");
    }
    return acos((v1[0] * v2[0] + v1[1] * v2[1]) / sqrt((v1[0] * v1[0] + v1[1] * v1[1]) * (v2[0] * v2[0] + v2[1] * v2[1])));
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "_vector package";
    m.def("angle_between",
          &angle_between,
          "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.");
}