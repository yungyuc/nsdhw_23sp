#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
namespace py = pybind11;

#include "Matrix.h"
#include "Multiply.h"

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "_matrix package";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<const size_t &, const size_t &>())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__getitem__", [](const Matrix &mat, py::tuple index)
             { 
                size_t it = index[0].cast<size_t>(); 
                size_t jt = index[1].cast<size_t>(); 
                return mat(it, jt); })
        .def("__setitem__", [](Matrix &mat, py::tuple index, double value)
             {
                size_t it = index[0].cast<size_t>();
                size_t jt = index[1].cast<size_t>();
                mat(it, jt) = value; })
        .def_property_readonly("nrow", [](const Matrix &mat)
                               { return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat)
                               { return mat.ncol(); });

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}