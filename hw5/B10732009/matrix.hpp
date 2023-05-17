#pragma once

#include <cstddef>
#include <vector>

class Matrix
{
private:
  std::size_t mRow;
  std::size_t mCol;
  double *mBuffer = nullptr;

public:
  Matrix();
  Matrix(std::size_t row, std::size_t col);
  // Matrix(std::size_t row, std::size_t col, std::vector<double> const &buffer);
  Matrix(Matrix const &other);
  Matrix &operator=(Matrix const &other);
  ~Matrix();

  double operator()(std::size_t row, std::size_t col) const;
  double &operator()(std::size_t row, std::size_t col);

  bool operator==(Matrix const &other) const;

  std::size_t nrow() const;
  std::size_t ncol() const;
  double *buffer() const;
  std::size_t size() const;
};

Matrix multiply_naive(Matrix const &m1, Matrix const &m2);
Matrix multiply_tile(Matrix const &m1, Matrix const &m2, std::size_t tsize);
Matrix multiply_mkl(Matrix const &m1, Matrix const &m2);