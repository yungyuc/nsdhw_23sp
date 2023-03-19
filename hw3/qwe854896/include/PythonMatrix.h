#ifndef PYTHON_MATRIX_H_INCLUDED
#define PYTHON_MATRIX_H_INCLUDED

#include <cstddef>
#include <pybind11/pybind11.h>
#include "Matrix.h"

namespace py = pybind11;

class PythonMatrix : public Matrix
{
public:
    PythonMatrix(const size_t &nrow, const size_t &ncol);
    PythonMatrix(const PythonMatrix &mat);
    PythonMatrix(const Matrix &mat);
    ~PythonMatrix();

    double getitem(py::tuple index) const;
    void setitem(py::tuple index, double value);

    const size_t nrow;
    const size_t ncol;

    bool operator==(const PythonMatrix &other) const;
    bool operator!=(const PythonMatrix &other) const;
};

#endif