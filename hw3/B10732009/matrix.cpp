#include <cstring>
#include <stdexcept>

#include "matrix.hpp"

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

std::size_t Matrix::row() const
{
  return mRow;
}

std::size_t Matrix::col() const
{
  return mCol;
}

std::size_t Matrix::size() const
{
  return mRow * mCol;
}