#include <cstring>
#include <stdexcept>

#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>

#include "_matrix.hpp"

Matrix::Matrix() : mRow(0), mCol(0), mBuffer(nullptr)
{
}

Matrix::Matrix(std::size_t row, std::size_t col) : mRow(row), mCol(col), mBuffer(new double[row * col]())
{
}

Matrix::Matrix(std::size_t row, std::size_t col, std::vector<double> const &buffer)
{
  // check size correctness
  if (row * col != buffer.size())
    throw std::out_of_range("number of elements mismatch");
  // copy data
  mRow = row;
  mCol = col;
  mBuffer = new double[buffer.size()];
  std::memcpy(mBuffer, buffer.data(), buffer.size() * sizeof(double));
}

Matrix::Matrix(Matrix const &other)
{
  (*this) = other;
}

Matrix &Matrix::operator=(Matrix const &other)
{
  if (this != &other)
  {
    // free old buffer
    if (mBuffer)
      delete[] mBuffer;
    // copy data
    mRow = other.row();
    mCol = other.col();
    if (other.size() > 0)
    {
      mBuffer = new double[other.size()];
      std::memcpy(mBuffer, other.mBuffer, other.size() * sizeof(double));
    }
    else
      mBuffer = nullptr;
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
  for (std::size_t i = 0; i < mRow * mCol; i++)
  {
    if (mBuffer[i] != other.mBuffer[i])
      return false;
  }
  return true;
}

std::size_t Matrix::row() const
{
  return mRow;
}

std::size_t Matrix::col() const
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
  if (m1.col() != m2.row())
    throw std::out_of_range("dimension mismatch");

  Matrix ret(m1.row(), m2.col());

  for (std::size_t i = 0; i < m1.row(); i++)
  {
    for (std::size_t j = 0; j < m2.col(); j++)
    {
      for (std::size_t k = 0; k < m1.col(); k++)
        ret(i, j) += m1(i, k) * m2(k, j);
    }
  }

  return ret;
}

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, std::size_t tsize)
{
  if (m1.col() != m2.row())
    throw std::out_of_range("dimension mismatch");

  Matrix ret(m1.row(), m2.col());

  for (std::size_t a = 0; a < m1.row(); a += tsize)
  {
    for (std::size_t c = 0; c < m1.col(); c += tsize)
    {
      for (std::size_t b = 0; b < m2.col(); b += tsize)
      {
        for (std::size_t i = a; i < std::min(a + tsize, m1.row()); i++)
        {
          for (std::size_t k = c; k < std::min(c + tsize, m1.col()); k++)
          {
            for (std::size_t j = b; j < std::min(b + tsize, m2.col()); j++)
              ret(i, j) += m1(i, k) * m2(k, j);
          }
        }
      }
    }
  }

  return ret;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
  if (mat1.col() != mat2.row())
    throw std::out_of_range("dimension mismatch");

  Matrix ret(mat1.row(), mat2.col());
  cblas_dgemm(CblasRowMajor, /* const CBLAS_LAYOUT Layout */
              CblasNoTrans,  /* const CBLAS_TRANSPOSE transa */
              CblasNoTrans,  /* const CBLAS_TRANSPOSE transb */
              mat1.row(),    /* const MKL_INT m */
              mat2.col(),    /* const MKL_INT n */
              mat1.col(),    /* const MKL_INT k */
              1.0,           /* const double alpha */
              mat1.buffer(), /* const double *a */
              mat1.col(),    /* const MKL_INT lda */
              mat2.buffer(), /* const double *b */
              mat2.col(),    /* const MKL_INT ldb */
              0.0,           /* const double beta */
              ret.buffer(),  /* double * c */
              ret.col()      /* const MKL_INT ldc */
  );

  return ret;
}

#define PYBIND11

#ifdef PYBIND11
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;
PYBIND11_MODULE(_matrix, m)
{
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::vector<double>>())
      .def_property_readonly("nrow", [](Matrix const &m) { return m.row(); })
      .def_property_readonly("ncol", [](Matrix const &m) { return m.col(); })
      .def("__setitem__", [](Matrix &m, std::vector<std::size_t> idx, double val) { m(idx[0], idx[1]) = val; })
      .def("__getitem__", [](Matrix const &m, std::vector<std::size_t> idx) { return m(idx[0], idx[1]); })
      .def("__eq__", [](Matrix const &m1, Matrix const &m2) { return (m1 == m2); });

  m.def("multiply_naive", &multiply_naive, "");
  m.def("multiply_tile", &multiply_tile, "");
  m.def("multiply_mkl", &multiply_mkl, "");
}
#endif
