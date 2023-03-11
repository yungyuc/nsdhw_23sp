#include <pybind11/pybind11.h>
#include <cmath>
using namespace std;
namespace py = pybind11;

double angle(double x1, double y1, double x2, double y2)
{
    return acos((x1 * x2 + y1 * y2) / sqrt((x1 * x1 + y1 * y1) * (x2 * x2 + y2 * y2)));
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "_vector package";
    m.def("angle",
          &angle,
          "calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.");
}