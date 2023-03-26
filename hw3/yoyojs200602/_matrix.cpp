#include <iostream>
#include <vector>
#include "mkl.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


class Matrix {
public:
    Matrix(): nrow_(0), ncol_(0) {}
    Matrix(int nrow, int ncol): nrow_(nrow), ncol_(ncol), data_(nrow * ncol) {}

    int nrow() const { return nrow_; }
    int ncol() const { return ncol_; }

    double& operator()(int i, int j) { return data_[i * ncol_ + j]; }
    const double& operator()(int i, int j) const { return data_[i * ncol_ + j]; }
    double* data() { return data_.data();}
    const double* data() const { return data_.data();}

private:
    int nrow_, ncol_;
    std::vector<double> data_;
};

Matrix multiply_naive(const Matrix& a, const Matrix& b) {
    Matrix c(a.nrow(), b.ncol());

    for (int i = 0; i < a.nrow(); ++i) {
        for (int j = 0; j < b.ncol(); ++j) {
            double sum = 0.0;
            for (int k = 0; k < a.ncol(); ++k) {
                sum += a(i, k) * b(k, j);
            }
            c(i, j) = sum;
        }
    }

    return c;
}


Matrix multiply_tile(const Matrix& a, const Matrix& b, int tile_size) {
    Matrix c(a.nrow(), b.ncol());

    for (int i0 = 0; i0 < a.nrow(); i0 += tile_size) {
        int i1 = std::min(i0 + tile_size, a.nrow());
        for (int j0 = 0; j0 < b.ncol(); j0 += tile_size) {
            int j1 = std::min(j0 + tile_size, b.ncol());
            for (int k0 = 0; k0 < a.ncol(); k0 += tile_size) {
                int k1 = std::min(k0 + tile_size, a.ncol());
                for (int i = i0; i < i1; ++i) {
                    for (int j = j0; j < j1; ++j) {
                        double sum = 0.0;
                        for (int k = k0; k < k1; ++k) {
                            sum += a(i, k) * b(k, j);
                        }
                        c(i, j) += sum;
                    }
                }
            }
        }
    }

    return c;
}

Matrix multiply_mkl(const Matrix& a, const Matrix& b) {
    Matrix c(a.nrow(), b.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                a.nrow(), b.ncol(), a.ncol(), 1.0,
                a.data(), a.ncol(), b.data(), b.ncol(),
                0.0, c.data(), c.ncol());

    return c;

    // Matrix c(a.nrow(), b.ncol());

    // for (int i = 0; i < a.nrow(); ++i) {
    //     for (int j = 0; j < b.ncol(); ++j) {
    //         double sum = 0.0;
    //         for (int k = 0; k < a.ncol(); ++k) {
    //             sum += a(i, k) * b(k, j);
    //         }
    //         c(i, j) = sum;
    //     }
    // }

    // return c;
}


PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "matrix multiplication testing";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def("data", pybind11::overload_cast<>(&Matrix::data))
        .def("data", pybind11::overload_cast<>(&Matrix::data, pybind11::const_))
        // .def("__call__", pybind11::overload_cast<int, int>(&Matrix::operator()), pybind11::return_value_policy::reference_internal)
        // .def("__call__", pybind11::overload_cast<int, int>(&Matrix::operator(), pybind11::const_), pybind11::return_value_policy::reference_internal)
        .def("__setitem__", [](Matrix &self, std::pair<int, int> id, double val) { self(id.first, id.second) = val; })
        .def("__getitem__", [](const Matrix &self, std::pair<int, int> id) { return self(id.first, id.second); })
        .def_property_readonly("nrow", [](const Matrix &mat) { return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat) { return mat.ncol(); });
}