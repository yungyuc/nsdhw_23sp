#include <pybind11/pybind11.h>
#include <cmath>
#include <pybind11/stl.h>

namespace py = pybind11;

double getAngle(std::vector<double> p1, std::vector<double> p2) {
  double lenOfP1, lenOfP2;
  lenOfP1 = sqrt(p1[0] * p1[0] + p1[1] * p1[1]);
  lenOfP2 = sqrt(p2[0] * p2[0] + p2[1] * p2[1]);

  double cosTheta = (p1[0] * p2[0] + p1[1] * p2[1]) / (lenOfP1 * lenOfP2);
  return acos(cosTheta);
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "Get angle function implementation";      // module doc string
  m.def("getAngle",                              // function name
    &getAngle,                               // function pointer
    "A function which return the radian of two 2-d vectors" //function doc string
  );
}