#include <bits/stdc++.h>
#include <pybind11/pybind11.h>
using namespace std;

double cal_rad(pair<double, double> v1, pair<double, double> v2) {
  double v1_len = sqrt(v1.first * v1.first + v1.second * v1.second);
  double v2_len = sqrt(v2.first * v2.first + v2.second * v2.second);

  if (v1_len == 0 || v2_len == 0)
    return -1;

  double cos_theta =
      (v1.first * v2.first + v1.second * v2.second) / (v1_len * v2_len);
  return acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 example plugin"; // optional module docstring

  m.def("cal_rad", &cal_rad,
        "A function that calculates the angle (in radians) between two vectors "
        "in the 2-dimensional Cartesian coordinate system.");
}
