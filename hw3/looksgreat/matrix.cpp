#include "matrix.hpp"
#include<bits/stdc++.h>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>
using namespace std;

Matrix multiply_naive(Matrix &m1, Matrix &m2){
    Matrix ret(m1.rows(), m2.cols());
    for (int i = 0; i < m1.rows(); i++) {
        for (int j = 0; j < m2.cols(); j++) {
            double sum = 0.0;
            for (int k = 0; k < m1.cols(); k++) {
                sum += m1(i, k) * m2(k, j);
            }
            ret(i, j) = sum;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, int const tile_size){
    Matrix ret(m1.rows(), m2.cols());
    int m = m1.rows();
    int n = m2.cols();
    int k = m1.cols();

    for (int i0 = 0; i0 < m; i0 += tile_size) {
        int imax = min(i0 + tile_size, m);
        for (int j0 = 0; j0 < n; j0 += tile_size) {
            int jmax = min(j0 + tile_size, n);
            for (int k0 = 0; k0 < k; k0 += tile_size) {
                int kmax = min(k0 + tile_size, k);
                for (int i = i0; i < imax; i++) {
                    for (int j = j0; j < jmax; j++) {
                        double sum = 0.0;
                        for (int l = k0; l < kmax; l++) {
                            sum += m1(i, l) * m2(l, j);
                        }
                        ret(i, j) += sum;
                    }
                }
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix const &m1, Matrix const &m2){
    mkl_set_num_threads(1);
    Matrix ret(m1.rows(), m2.cols());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.rows(),  m2.cols(), m1.cols(), 1.0 , m1.buffer(),
     m1.cols(), m2.buffer(), m2.cols(), 0.0, ret.buffer(), ret.cols());
    return ret;
    /*Matrix ret(m1.rows(), m2.cols());
    for (int i = 0; i < m1.rows(); i++) {
        for (int j = 0; j < m2.cols(); j++) {
            double sum = 0.0;
            for (int k = 0; k < m1.cols(); k++) {
                sum += m1(i, k) * m2(k, j);
            }
            ret(i, j) = sum;
        }
    }
    return ret;*/
}

PYBIND11_MODULE(_matrix, m){
    m.doc() = "matrix-matrix multiplication";

    m.def("multiply_naive", &multiply_naive, "naive");
    m.def("multiply_tile", &multiply_tile, "tile");
    m.def("multiply_mkl", &multiply_mkl, "mkl");
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