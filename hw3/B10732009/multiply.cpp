#include <cstddef>
#include <stdexcept>

#include "matrix.hpp"

Matrix multiply_naive(Matrix const &m1, Matrix const &m2)
{
  // dimension check
  if (m1.col() != m2.row())
    throw std::out_of_range("dimension mismatch");

  Matrix ret(m1.row(), m2.col());
  for (std::size_t i = 0; i < m1.row(); i++)
  {
    for (std::size_t j = 0; j < m2.col(); j++)
    {
      double temp = 0.0;
      for (std::size_t k = 0; k < m1.col(); k++)
        temp += m1(i, k) * m2(k, j);
      ret(i, j) = temp;
    }
  }
  return ret;
}