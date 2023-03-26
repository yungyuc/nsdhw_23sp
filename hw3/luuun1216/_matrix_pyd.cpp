#include "_matrix.hpp"
#include <pybind11/pybind11.h>

namespace pybind = pybind11;

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiply";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    pybind::class_<Matrix>(m, "Matrix")
        .def(pybind::init<int, int>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
        .def("assign", &Matrix::operator=)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> idx, double val) { return mat(idx.first, idx.second) = val; })
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> idx) { return mat(idx.first, idx.second); });
}