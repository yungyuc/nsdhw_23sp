#include <cstring>
#include <stdexcept>

#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>

#include "matrix.hpp"

Matrix::Matrix() : mRow(0), mCol(0), mBuffer(nullptr)
{
}

Matrix::Matrix(std::size_t row, std::size_t col) : mRow(row), mCol(col), mBuffer(new double[row * col]())
{
}

Matrix::Matrix(Matrix const &other)
{
  (*this) = other;
}

Matrix &Matrix::operator=(Matrix const &other)
{
  if (this != &other)
  {
    if (mBuffer)
      delete[] mBuffer;

    mRow = other.nrow();
    mCol = other.ncol();
    mBuffer = (other.size() == 0) ? (nullptr) : (new double[other.size()]);
    std::memcpy(mBuffer, other.mBuffer, other.size() * sizeof(double));
  }
  return (*this);
}

Matrix::~Matrix()
{
  if (mBuffer)
    delete[] mBuffer;
}

double Matrix::operator()(std::size_t row, std::size_t col) const
{
  if (row < 0 || row >= mRow || col < 0 || col > mCol)
    throw std::out_of_range("Index out of range");
  return mBuffer[row * mCol + col];
}

double &Matrix::operator()(std::size_t row, std::size_t col)
{
  if (row < 0 || row >= mRow || col < 0 || col > mCol)
    throw std::out_of_range("Index out of range");
  return mBuffer[row * mCol + col];
}

bool Matrix::operator==(Matrix const &other) const
{
  if (mRow != other.mRow || mCol != other.mCol)
    return false;
  for (std::size_t i = 0; i < size(); i++)
  {
    if (mBuffer[i] != other.mBuffer[i])
      return false;
  }
  return true;
}

std::size_t Matrix::nrow() const
{
  return mRow;
}

std::size_t Matrix::ncol() const
{
  return mCol;
}

double *Matrix::buffer() const
{
  return mBuffer;
}

std::size_t Matrix::size() const
{
  return mRow * mCol;
}

Matrix multiply_naive(Matrix const &m1, Matrix const &m2)
{
  if (m1.ncol() != m2.nrow())
    throw std::out_of_range("Dimension mismatch");

  Matrix ret(m1.nrow(), m2.ncol());

  for (std::size_t i = 0; i < m1.nrow(); i++)
  {
    for (std::size_t j = 0; j < m2.ncol(); j++)
    {
      for (std::size_t k = 0; k < m1.ncol(); k++)
        ret(i, j) += m1(i, k) * m2(k, j);
    }
  }

  return ret;
}

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, std::size_t tsize)
{
  if (m1.ncol() != m2.nrow())
    throw std::out_of_range("Dimension mismatch");

  Matrix ret(m1.nrow(), m2.ncol());

  for (std::size_t a = 0; a < m1.nrow(); a += tsize)
  {
    for (std::size_t c = 0; c < m1.ncol(); c += tsize)
    {
      for (std::size_t b = 0; b < m2.ncol(); b += tsize)
      {
        for (std::size_t i = a; i < std::min(a + tsize, m1.nrow()); i++)
        {
          for (std::size_t k = c; k < std::min(c + tsize, m1.ncol()); k++)
          {
            for (std::size_t j = b; j < std::min(b + tsize, m2.ncol()); j++)
              ret(i, j) += m1(i, k) * m2(k, j);
          }
        }
      }
    }
  }

  return ret;
}

Matrix multiply_mkl(Matrix const &m1, Matrix const &m2)
{
  if (m1.ncol() != m2.nrow())
    throw std::out_of_range("Dimension mismatch");

  Matrix ret(m1.nrow(), m2.ncol());
  cblas_dgemm(CblasRowMajor, /* const CBLAS_LAYOUT Layout */
              CblasNoTrans,  /* const CBLAS_TRANSPOSE transa */
              CblasNoTrans,  /* const CBLAS_TRANSPOSE transb */
              m1.nrow(),     /* const MKL_INT m */
              m2.ncol(),     /* const MKL_INT n */
              m1.ncol(),     /* const MKL_INT k */
              1.0,           /* const double alpha */
              m1.buffer(),   /* const double *a */
              m1.ncol(),     /* const MKL_INT lda */
              m2.buffer(),   /* const double *b */
              m2.ncol(),     /* const MKL_INT ldb */
              0.0,           /* const double beta */
              ret.buffer(),  /* double * c */
              ret.ncol()     /* const MKL_INT ldc */
  );

  return ret;
}

// #define PYBIND11

// #ifdef PYBIND11
// #include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// namespace py = pybind11;
// PYBIND11_MODULE(_matrix, m)
// {
//   py::class_<Matrix>(m, "Matrix")
//       .def(py::init<std::size_t, std::size_t>())
//       .def_property_readonly("nrow", [](Matrix const &m) { return m.nrow(); })
//       .def_property_readonly("ncol", [](Matrix const &m) { return m.ncol(); })
//       .def("__setitem__", [](Matrix &m, std::vector<std::size_t> idx, double val) { m(idx[0], idx[1]) = val; })
//       .def("__getitem__", [](Matrix const &m, std::vector<std::size_t> idx) { return m(idx[0], idx[1]); })
//       .def("__eq__", [](Matrix const &m1, Matrix const &m2) { return (m1 == m2); });

//   m.def("multiply_naive", &multiply_naive, "");
//   m.def("multiply_tile", &multiply_tile, "");
//   m.def("multiply_mkl", &multiply_mkl, "");
// }
// #endif