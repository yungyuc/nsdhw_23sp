#include "PythonMatrix.h"

PythonMatrix::PythonMatrix(const size_t &nrow, const size_t &ncol)
    : Matrix(nrow, ncol), nrow(nrow), ncol(ncol) {}

PythonMatrix::PythonMatrix(const PythonMatrix &mat)
    : Matrix(mat), nrow(mat.nrow), ncol(mat.ncol) {}

PythonMatrix::PythonMatrix(const Matrix &mat)
    : Matrix(mat), nrow(mat.nrow()), ncol(mat.ncol()) {}

PythonMatrix::~PythonMatrix() {}

double PythonMatrix::getitem(py::tuple index) const
{
    size_t it = index[0].cast<size_t>();
    size_t jt = index[1].cast<size_t>();
    return this->operator()(it, jt);
}

void PythonMatrix::setitem(py::tuple index, double value)
{
    size_t it = index[0].cast<size_t>();
    size_t jt = index[1].cast<size_t>();
    this->operator()(it, jt) = value;
}

bool PythonMatrix::operator==(const PythonMatrix &other) const
{
    return Matrix::operator==(other);
}
bool PythonMatrix::operator!=(const PythonMatrix &other) const
{
    return Matrix::operator!=(other);
}