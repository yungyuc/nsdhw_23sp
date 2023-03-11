#include<pybind11/pybind11.h>
#include<iostream>
#include<vector>
#include<utility>
#include<cmath>
#include<sstream>

#define ZERO_ABS 1e-11

namespace py=pybind11;

class _vector{
    public:
        double x;
        double y;
        _vector(){
            x = 0;
            y = 0;
        }
        _vector(std::pair<double, double> p) {
            x = p.first;
            y = p.second;
        }
        _vector(double x, double y) {
            this->x = x;
            this->y = y;
        }
};

double angle(_vector v1, _vector v2) {
    if(std::fpclassify(v1.x) == FP_ZERO && std::fpclassify(v1.y) == FP_ZERO) {
        throw std::invalid_argument("vector length should greater than zero");
    }
    if(std::fpclassify(v2.x) == FP_ZERO && std::fpclassify(v2.y) == FP_ZERO) {
        throw std::invalid_argument("vector length should greater than zero");
    }
    return acos((v1.x * v2.x + v1.y * v2.y) / sqrt((v1.x * v1.x + v1.y * v1.y)) * sqrt((v2.x * v2.x + v2.y * v2.y)));
}
PYBIND11_MODULE(_vector, m) {
    py::class_<_vector>(m, "vector")
        .def(py::init<>())
        .def(py::init<std::pair<double, double>>())
        .def(py::init<double, double>())
        .def("__str__", [](_vector const &v) {
            std::stringstream ss;
            ss << "(" << v.x << ", " << v.y << ")";
            return ss.str();
        })
        .def("__repr__", [](_vector const &v) {
            std::stringstream ss;
            ss << "(" << v.x << ", " << v.y << ")";
            return ss.str();
        })
        .def_readwrite("x", &_vector::x)
        .def_readwrite("y", &_vector::y);
    m.def("angle", &angle);
}
