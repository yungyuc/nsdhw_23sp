#include <bits/stdc++.h>
#include <pybind11/pybind11.h>
#include "Matrix.h"

#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>

namespace py = pybind11;

Matrix multiply_naive(Matrix &a, Matrix &b)
{
    if (a.ncol() != b.nrow()) {
        exit(1);
    }

    Matrix c(a.nrow(), b.ncol());
    for (size_t i = 0; i < a.nrow(); i++) {
        for (size_t j = 0; j < b.ncol(); j++) {
            for (size_t k = 0; k < a.ncol(); k++) {
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
    for (size_t i = 0; i < a.nrow(); i += tile_size) {
        for (size_t k = 0; k < a.ncol(); k += tile_size) {
            for (size_t j = 0; j < b.ncol(); j += tile_size) {
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

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol() != mat2.nrow()) {
        exit(1);
    }

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transa */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transb */
                ,
                mat1.nrow() /* const MKL_INT m */
                ,
                mat2.ncol() /* const MKL_INT n */
                ,
                mat1.ncol() /* const MKL_INT k */
                ,
                1.0 /* const double alpha */
                ,
                mat1.m_buffer.data() /* const double *a */
                ,
                mat1.ncol() /* const MKL_INT lda */
                ,
                mat2.m_buffer.data() /* const double *b */
                ,
                mat2.ncol() /* const MKL_INT ldb */
                ,
                0.0 /* const double beta */
                ,
                ret.m_buffer.data() /* double * c */
                ,
                ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}

std::size_t bytes()
{
    return alloc.bytes();
}
std::size_t allocated()
{
    return alloc.allocated();
}
std::size_t deallocated()
{
    return alloc.deallocated();
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "matrix-matrix multiplication";

    m.def("multiply_naive", &multiply_naive, "multiply_naive");
    m.def("multiply_tile", &multiply_tile, "multiply_tile");
    m.def("multiply_mkl", &multiply_mkl, "multiply_mkl");
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", [](const Matrix &a, const Matrix &b) { return a == b; })
        .def("__setitem__",
             [](Matrix &self, std::pair<size_t, size_t> idx, double val) {
                 self(idx.first, idx.second) = val;
             })
        .def("__getitem__",
             [](const Matrix &self, std::pair<size_t, size_t> idx) {
                 return self(idx.first, idx.second);
             });
}
