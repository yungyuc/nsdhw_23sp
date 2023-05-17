#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
namespace py = pybind11;

#include "matrix.hpp"
#include "matrix_multiply.hpp"

PYBIND11_MODULE(_matrix, m)
{
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<const Matrix &>())
        .def("assign", &Matrix::operator=)
        .def("__getitem__",
             [](const Matrix &m, std::pair<size_t, size_t> idx) {
                 return m(idx.first, idx.second);
             })
        .def("__setitem__",
             [](Matrix &m, std::pair<size_t, size_t> idx, double value) {
                 m(idx.first, idx.second) = value;
             })
        .def_property_readonly("nrow", [](const Matrix &mat)
                               { return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat)
                               { return mat.ncol(); })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("zero", &Matrix::zero);

    m.def("multiply_naive", &MatrixMultiply::multiply_naive);
    m.def("multiply_tile", &MatrixMultiply::multiply_tile);
    m.def("multiply_mkl", &MatrixMultiply::multiply_mkl);
}