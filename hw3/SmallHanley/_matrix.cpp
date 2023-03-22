#include <bits/stdc++.h>
#include <pybind11/pybind11.h>
#include "Matrix.h"

namespace py = pybind11;

Matrix multiply_naive(Matrix &a, Matrix &b)
{
    if (a.ncol() != b.nrow()) {
        exit(1);
    }

    Matrix c(a.nrow(), b.ncol());
    for (size_t i = 0; i < a.nrow(); i++) {
        for (size_t k = 0; k < a.ncol(); i++) {
            for (size_t j = 0; j < b.ncol(); j++) {
                c(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return c;
}

Matrix multiply_tile(Matrix const &a, Matrix const &b, size_t const tile_size)
{
    if (a.ncol() != b.nrow()) {
        exit(1);
    }

    Matrix c(a.nrow(), b.ncol());
    for (size_t i = 0; i < a.nrow(); i++) {
        for (size_t k = 0; k < a.ncol(); k++) {
            for (size_t j = 0; j < b.ncol(); j++) {
                for (size_t ti = i; ti < std::min(i + tile_size, a.nrow());
                     ti++) {
                    for (size_t tk = k; tk < std::min(k + tile_size, a.ncol());
                         tk++) {
                        for (size_t tj = j;
                             tj < std::min(j + tile_size, b.ncol()); tj++) {
                            c(ti, tj) += a(ti, tk) * b(tk, tj);
                        }
                    }
                }
            }
        }
    }
    return c;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "matrix-matrix multiplication";

    m.def("multiply_naive", &multiply_naive, "multiply_naive");
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", [](const Matrix &a, const Matrix &b) { return a == b; })
        .def("__setitem__",
             [](Matrix &self, std::pair<size_t, size_t> idx, double val) {
                 return self(idx.first, idx.second) = val;
             })
        .def("__getitem__",
             [](const Matrix &self, std::pair<size_t, size_t> idx) {
                 return self(idx.first, idx.second);
             });
}
