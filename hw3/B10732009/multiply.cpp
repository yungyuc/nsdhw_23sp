#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "matrix.hpp"

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2)
{
  // dimension check
  if (mat1.col() != mat2.row())
    throw std::out_of_range("dimension mismatch");

  Matrix ret(mat1.row(), mat2.col());

  for (std::size_t i = 0; i < mat1.row(); i++)
  {
    for (std::size_t j = 0; j < mat2.col(); j++)
    {
      double temp = 0.0;
      for (std::size_t k = 0; k < mat1.col(); k++)
        temp += mat1(i, k) * mat2(k, j);
      ret(i, j) = temp;
    }
  }

  return ret;
}

Matrix multiple_tile(Matrix const &mat1, Matrix const &mat2, std::size_t tsize)
{
  // dimension check
  if (mat1.col() != mat2.row())
    throw std::out_of_range("dimension mismatch");

  Matrix ret(mat1.row(), mat2.col());

  // 2 layers for-loop to iterate the result block index
  for (std::size_t rr = 0; rr < ret.row(); rr += tsize)
  {
    for (std::size_t rc = 0; rc < ret.col(); rc += tsize)
    {
      // 1 layer for-loop to dot-product the corresponding blocks
      for (std::size_t d = 0; d < mat1.col(); d += tsize)
      {
        // 3 layers for-loop to do naive multiplication in corresponding range
        for (std::size_t i = rr; i < std::min(rr + tsize, mat1.row()); i++)
        {
          for (std::size_t j = rc; j < std::min(rc + tsize, mat2.col()); j++)
          {
            double temp = 0.0;
            for (std::size_t k = d; k < std::min(d + tsize, mat1.col()); k++)
              temp += mat1(i, k) * mat2(k, j);
            ret(i, j) += temp;
          }
        }
      }
    }
  }

  return ret;
}
