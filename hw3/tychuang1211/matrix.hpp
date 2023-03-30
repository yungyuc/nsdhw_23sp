#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <sstream>

#include <mkl.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol);
    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);

    Matrix & operator=(std::vector<double> const & vec);
    Matrix(Matrix const & other);
    Matrix & operator=(Matrix const & other);
    Matrix(Matrix && other);
    Matrix & operator=(Matrix && other);

    ~Matrix()
    {
        reset_buffer(0, 0);
    };

    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }
    bool operator==(const Matrix &other) const;

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double * buffer(size_t i) const { return &m_buffer[i]; }
    std::vector<double> buffer_vector() const
    {
        return std::vector<double>(m_buffer, m_buffer+size());
    }

private:

    inline size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

std::ostream & operator << (std::ostream & ostr, Matrix const & mat);

PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, std::vector<double> const &>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def(py::self == py::self)
        .def("__setitem__", [](Matrix & mat, std::pair<size_t, size_t> index, double item) {
                mat(index.first, index.second) = item;
            }
        )
        .def("__getitem__", 
            [](const Matrix & mat, std::pair<size_t, size_t> index) {
                return mat(index.first, index.second);
            }
        )
        .def("__repr__", 
            [](const Matrix & mat) {
                std::stringstream stream;
                stream << mat;
                return stream.str(); 
            }
        );

    m.def("multiply_naive", &multiply_naive, 
            "A function that performs naive matrix-matrix multiplication.");
    m.def("multiply_tile", &multiply_tile, 
            "A function that performs matrix-matrix multiplication with tiling.");
    m.def("multiply_mkl", &multiply_mkl, 
            "A function that uses DGEMM for matrix-matrix multiplication.");  
}