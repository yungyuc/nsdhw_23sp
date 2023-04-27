#include <stdexcept>
#include <cstddef>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>

#include <limits>

#include "matrix.hpp"
#include "counter_and_allocator.hpp"

namespace py = pybind11;

Matrix multiply_naive(const Matrix &lhs, const Matrix &rhs)
{
    if (lhs.n_col() != rhs.n_row())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    int r = lhs.n_row(), c = rhs.n_col(), n = lhs.n_col();

    Matrix result(r, c);

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                result(i, j) = result(i, j) + lhs(i, k) * rhs(k, j);
            }
        }
    }
    return result;
}

void loadBlock(const Matrix &m, double *buf, size_t r, size_t c, size_t blocksize)
{
    size_t rowStart = r * blocksize;
    size_t colStart = c * blocksize;

    size_t row, col;

    size_t idx = 0, cnt = 0;

    for (row = rowStart; row < std::min(rowStart + blocksize, m.n_row()); ++row)
    {
        for (col = colStart; col < std::min(colStart + blocksize, m.n_col()); ++col)
        {
            buf[idx] = m(rowStart, colStart);
            idx++;
        }
        cnt++;
        idx = cnt * blocksize;
    }
}

void mulBlock(double *lhsBuf, double *rhsBuf, size_t blocksize, double *resultBuf)
{
    for (size_t i = 0; i < blocksize; ++i)
    {
        for (size_t k = 0; k < blocksize; ++k)
        {
            for (size_t j = 0; j < blocksize; ++j)
            {
                resultBuf[i * blocksize + j] += lhsBuf[i * blocksize + k] * rhsBuf[k * blocksize + j];
            }
        }
    }
}

void saveBuf(double *resultBuf, Matrix *result, size_t blocksize, size_t r, size_t c)
{
    size_t rowStart = r * blocksize;
    size_t colStart = c * blocksize;

    size_t row, col;

    size_t idx = 0, cnt = 0;

    for (row = rowStart; row < std::min(rowStart + blocksize, (*result).n_row()); ++row)
    {
        for (col = colStart; col < std::min(colStart + blocksize, (*result).n_col()); ++col)
        {
            (*result)(row, col) = resultBuf[idx];
            idx++;
        }
        cnt++;
        idx = cnt * blocksize;
    }
}

Matrix multiply_tile(const Matrix &lhs, const Matrix &rhs, size_t blocksize)
{
    if (lhs.n_col() != rhs.n_row())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    Matrix *result = new Matrix(lhs.n_row(), rhs.n_col());

    double lhsBuf[blocksize * blocksize];
    std::fill(lhsBuf, lhsBuf + blocksize * blocksize, 0);

    double rhsBuf[blocksize * blocksize];
    std::fill(rhsBuf, rhsBuf + blocksize * blocksize, 0);

    double resultBuf[blocksize * blocksize];
    std::fill(resultBuf, resultBuf + blocksize * blocksize, 0);

    size_t lhsRow = lhs.n_row() / blocksize + (lhs.n_row() % blocksize > 0);
    size_t lhsCol = lhs.n_col() / blocksize + (lhs.n_col() % blocksize > 0);
    size_t rhsRow = rhs.n_row() / blocksize + (rhs.n_row() % blocksize > 0);
    size_t rhsCol = rhs.n_col() / blocksize + (rhs.n_col() % blocksize > 0);

    for (size_t i = 0; i < lhsRow; ++i)
    {
        for (size_t j = 0; j < rhsCol; ++j)
        {
            for (size_t k = 0; k < lhsCol; ++k)
            {
                loadBlock(lhs, lhsBuf, i, k, blocksize);
                loadBlock(rhs, rhsBuf, k, j, blocksize);
                mulBlock(lhsBuf, rhsBuf, blocksize, resultBuf);
                saveBuf(resultBuf, result, blocksize, i, j);
            }
        }
    }

    return *result;
}

Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.n_col() != mat2.n_row())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }
    Matrix ret(mat1.n_row(), mat2.n_col());

    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.n_row(), mat2.n_col(), mat1.n_col(), 1.0, mat1.m_buffer.data(), mat1.n_col(), mat2.m_buffer.data(), mat2.n_col(), 0.0, ret.m_buffer.data(), ret.n_col());

    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiplication function implementation"; // module doc string
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", [](Matrix &m)
                               { return m.n_row(); })
        .def_property_readonly("ncol", [](Matrix &m)
                               { return m.n_col(); })
        .def(py::self == py::self) // eq
        .def("__getitem__", [](Matrix &m, std::pair<size_t, size_t> idx)
             { return m(idx.first, idx.second); })
        .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> idx, double val)
             { m(idx.first, idx.second) = val; });
    m.def("bytes", &CustomAllocator<double>::bytes);
    m.def("allocated", &CustomAllocator<double>::allocated);
    m.def("deallocated", &CustomAllocator<double>::deallocated);
}
