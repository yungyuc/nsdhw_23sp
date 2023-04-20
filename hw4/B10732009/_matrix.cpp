// #include <atomic>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

#include <mkl.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

template <class T> class CustomAllocator
{
private:
  static std::size_t mByte;        // used bytes
  static std::size_t mAllocated;   // allocated bytes
  static std::size_t mDeallocated; // deallocated bytes

public:
  using value_type = T;
  CustomAllocator() = default;
  T *allocate(std::size_t n);
  void deallocate(T *ptr, std::size_t n);
  static std::size_t bytes();
  static std::size_t allocated();
  static std::size_t deallocated();
};

// initialize static variables
template <class T> std::size_t CustomAllocator<T>::mByte = 0;
template <class T> std::size_t CustomAllocator<T>::mAllocated = 0;
template <class T> std::size_t CustomAllocator<T>::mDeallocated = 0;

template <class T> T *CustomAllocator<T>::allocate(std::size_t n)
{
  // check if the size is reasonable
  if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
    throw std::bad_alloc();

  // allocate
  const std::size_t bytes = n * sizeof(T);
  T *ptr = (T *)(std::malloc(bytes));

  // check if allocation is successful
  if (ptr == nullptr)
    throw std::bad_alloc();

  // count bytes
  mByte += bytes;
  mAllocated += bytes;

  return ptr;
}

template <class T> void CustomAllocator<T>::deallocate(T *ptr, std::size_t n)
{
  const std::size_t bytes = n * sizeof(T);

  // free the memory
  std::free(ptr);

  // count bytes
  mByte -= bytes;
  mDeallocated += bytes;
}

template <class T> std::size_t CustomAllocator<T>::bytes()
{
  return mByte;
}

template <class T> std::size_t CustomAllocator<T>::allocated()
{
  return mAllocated;
}

template <class T> std::size_t CustomAllocator<T>::deallocated()
{
  return mDeallocated;
}

class Matrix
{
private:
  std::size_t mRow;
  std::size_t mCol;
  std::vector<double, CustomAllocator<double>> mBuffer;

public:
  Matrix();
  Matrix(std::size_t row, std::size_t col);
  Matrix(std::size_t row, std::size_t col, std::vector<double> const &data);
  // Matrix(Matrix const &other) = default;
  // Matrix &operator=(Matrix const &other) = default;
  // ~Matrix() = default;

  double operator()(std::size_t row, std::size_t col) const;
  double &operator()(std::size_t row, std::size_t col);

  bool operator==(Matrix const &other) const;
  bool operator!=(Matrix const &other) const;

  std::size_t row() const;
  std::size_t col() const;
  double *buffer() const;
};

Matrix::Matrix() : mRow(0), mCol(0), mBuffer({})
{
}

Matrix::Matrix(std::size_t row, std::size_t col)
    : mRow(row), mCol(col), mBuffer(std::vector<double, CustomAllocator<double>>(row * col))
{
}

Matrix::Matrix(std::size_t row, std::size_t col, std::vector<double> const &data)
{
  // check size correctness
  if (row * col != data.size())
    throw std::out_of_range("Number of elements mismatch");
  // copy data
  mRow = row;
  mCol = col;
  mBuffer = std::vector<double, CustomAllocator<double>>(row * col);
  for (std::size_t i = 0; i < data.size(); i++)
    mBuffer[i] = data[i];
}

// Matrix::Matrix(Matrix const &other)
// {
//   (*this) = other;
// }

// Matrix &Matrix::operator=(Matrix const &other)
// {
//   // check self assignment
//   if (this != &other)
//   {
//     // copy data
//     mRow = other.row();
//     mCol = other.col();
//     for (std::size_t i = 0; i < other.mBuffer.size(); i++)
//       mBuffer[i] = other.mBuffer[i];
//   }
//   return (*this);
// }

// Matrix::~Matrix()
// {
// }

double Matrix::operator()(std::size_t row, std::size_t col) const
{
  if (row < 0 || row >= mRow || col < 0 || col >= mCol)
    throw std::out_of_range("Index out of range");
  return mBuffer[row * mCol + col];
}

double &Matrix::operator()(std::size_t row, std::size_t col)
{
  if (row < 0 || row >= mRow || col < 0 || col >= mCol)
    throw std::out_of_range("Index out of range");
  return mBuffer[row * mCol + col];
}

bool Matrix::operator==(Matrix const &other) const
{
  return (mRow == other.mRow) && (mCol == other.mCol) && (mBuffer == other.mBuffer);
}

bool Matrix::operator!=(Matrix const &other) const
{
  return !((*this) == other);
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
  return (double *)mBuffer.data();
}

Matrix multiply_naive(Matrix const &m1, Matrix const &m2)
{
  if (m1.col() != m2.row())
    throw std::out_of_range("Dimension mismatch");

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
    throw std::out_of_range("Dimension mismatch");

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
    throw std::out_of_range("Dimension mismatch");

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

PYBIND11_MODULE(_matrix, m)
{
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<>())
      .def(py::init<std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::vector<double>>())
      .def_property_readonly("nrow", [](Matrix const &m) { return m.row(); })
      .def_property_readonly("ncol", [](Matrix const &m) { return m.col(); })
      .def("__setitem__", [](Matrix &m, std::vector<std::size_t> idx, double val) { m(idx[0], idx[1]) = val; })
      .def("__getitem__", [](Matrix const &m, std::vector<std::size_t> idx) { return m(idx[0], idx[1]); })
      .def("__eq__", [](Matrix const &m1, Matrix const &m2) { return (m1 == m2); })
      .def("__ne__", [](Matrix const &m1, Matrix const &m2) { return (m1 != m2); });

  m.def("multiply_naive", &multiply_naive, "");
  m.def("multiply_tile", &multiply_tile, "");
  m.def("multiply_mkl", &multiply_mkl, "");

  m.def("bytes", &CustomAllocator<double>::bytes, "");
  m.def("allocated", &CustomAllocator<double>::allocated, "");
  m.def("deallocated", &CustomAllocator<double>::deallocated, "");
}