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
  Matrix(std::size_t row, std::size_t col, std::vector<double> const &buffer);
  Matrix(Matrix const &other);
  Matrix &operator=(Matrix const &other);
  ~Matrix();

  double operator()(std::size_t row, std::size_t col) const;
  double &operator()(std::size_t row, std::size_t col);

  std::size_t row() const;
  std::size_t col() const;
  std::size_t size() const;
};
