#include "Matrix_Mul.h"
#include "Matrix.h"

#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>
#include <cstddef>
#include <stdexcept>
#include <cstring>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiplication function implementation"; // module doc string
    m.def("multiply_naive", &multiply_naive, py::return_value_policy::take_ownership);
    m.def("multiply_tile", &multiply_tile, py::return_value_policy::take_ownership);
    m.def("multiply_mkl", &multiply_mkl, py::return_value_policy::take_ownership);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", [](Matrix &m)
                               { return m.n_row(); })
        .def_property_readonly("ncol", [](Matrix &m)
                               { return m.n_col(); })
        .def(py::self == py::self) // eq
        .def("__getitem__", [](Matrix &m, std::pair<size_t, size_t> idx)
             { return m(idx.first, idx.second); })
        .def("__setitem__", [](Matrix &m, const size_t elem, double val)
             { m(elem) = val; })
        .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> idx, double val)
             { m(idx.first, idx.second) = val; });
    m.def("bytes", &CustomAllocator<double>::bytes);
    m.def("allocated", &CustomAllocator<double>::allocated);
    m.def("deallocated", &CustomAllocator<double>::deallocated);
}