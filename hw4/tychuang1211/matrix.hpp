#pragma once

#include <atomic>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <stdexcept>
#include <sstream>

#include <mkl.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

template<typename T> 
class CustomAllocator {

public:

    using value_type = T;

    CustomAllocator() = default; // default constructor.
    CustomAllocator(CustomAllocator const & ) = default; // copy constructor.
    CustomAllocator(CustomAllocator       &&) = default; // move constructor.
    CustomAllocator & operator=(CustomAllocator const & ) = default; // copy assignment operator.
    CustomAllocator & operator=(CustomAllocator       &&) = default; // move assignment operator.
    ~CustomAllocator() = default;

    T* allocate( std::size_t n );
    void deallocate( T* p, std::size_t n ) noexcept;

    static std::size_t bytes() { return m_allocated - m_deallocated; }
    static std::size_t allocated() { return m_allocated; };
    static std::size_t deallocated() { return m_deallocated; };

private:

    static std::atomic_size_t m_allocated;
    static std::atomic_size_t m_deallocated;

};
template <class T> std::atomic_size_t CustomAllocator<T>::m_allocated = 0;
template <class T> std::atomic_size_t CustomAllocator<T>::m_deallocated = 0;

template <class T, class U>
bool operator==(const CustomAllocator<T> & a, const CustomAllocator<U> & b)
{
    return a == b;
}

template <class T, class U>
bool operator!=(const CustomAllocator<T> & a, const CustomAllocator<U> & b)
{
    return !(a == b);
}

class Matrix {

    friend Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
    friend Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
    friend Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

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

private:

    inline size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_buffer;

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

    m.def("bytes", &CustomAllocator<double>::bytes);
    m.def("allocated", &CustomAllocator<double>::allocated);
    m.def("deallocated", &CustomAllocator<double>::deallocated);
}