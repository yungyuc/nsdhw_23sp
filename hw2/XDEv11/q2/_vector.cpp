#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

double mag(const vector<double>& v) {
	double res{0};
	for (auto& x : v) res += x * x;
	return sqrt(res);
}

double angle(const vector<double>& v1, const vector<double>& v2) {
	return acos(inner_product(v1.begin(), v1.end(), v2.begin(), 0.) / (mag(v1) * mag(v2)));
}

PYBIND11_MODULE(_vector, m) {
	m.doc() = "pybind11 plugin"; // optional module docstring

	m.def("angle", &angle,
		"a C++ function that calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.",
		py::arg("v1"), py::arg("v2"));
}
