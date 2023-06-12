#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <vector>
#include <cstddef>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

class Matrix
{
public:
    Matrix();
    Matrix(size_t c, size_t r);

    ~Matrix();

    const size_t &nrow() const;
    const size_t &ncol() const;

    double operator()(size_t r, size_t c) const;
    double &operator()(size_t r, size_t c);

    double operator()(size_t idx) const;
    double &operator()(size_t idx);

    bool operator==(const Matrix &rhs) const;

    Matrix &operator=(const Matrix &rhs);

    size_t m_col, m_row;
    std::vector<double> m_buffer;

    py::array_t<double> array();
};

#endif