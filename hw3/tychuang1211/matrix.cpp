#include "matrix.hpp"

Matrix::Matrix(size_t nrow, size_t ncol)
  : m_nrow(nrow), m_ncol(ncol)
{
    reset_buffer(nrow, ncol);
}

Matrix::Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
    : m_nrow(nrow), m_ncol(ncol)
{
    reset_buffer(nrow, ncol);
    (*this) = vec;
}

Matrix & Matrix::operator=(std::vector<double> const & vec)
{
    if (size() != vec.size())
    {
        throw std::out_of_range("number of elements mismatch");
    }

    size_t k = 0;
    for (size_t i=0; i<m_nrow; ++i)
    {
        for (size_t j=0; j<m_ncol; ++j)
        {
            (*this)(i,j) = vec[k];
            ++k;
        }
    }

    return *this;
}

Matrix::Matrix(Matrix const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    reset_buffer(other.m_nrow, other.m_ncol);
    for (size_t i=0; i<m_nrow; ++i)
    {
        for (size_t j=0; j<m_ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
}

Matrix & Matrix::operator=(Matrix const & other)
{
    if (this == &other) { return *this; }
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
    }
    for (size_t i=0; i<m_nrow; ++i)
    {
        for (size_t j=0; j<m_ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
    return *this;
}

Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
}

Matrix & Matrix::operator=(Matrix && other)
{
    if (this == &other) { return *this; }
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    return *this;
}

bool Matrix::operator==(const Matrix &other) const
{
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        return false;

    for (size_t i = 0; i < m_nrow; ++i)
    {
        for (size_t j = 0; j < m_ncol; ++j)
        {
            if ((*this)(i, j) != other(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

void Matrix::reset_buffer(size_t nrow, size_t ncol)
{
    if (m_buffer) { delete[] m_buffer; }
    const size_t nelement = nrow * ncol;
    if (nelement) { m_buffer = new double[nelement](); }
    else          { m_buffer = nullptr; }
    m_nrow = nrow;
    m_ncol = ncol;
}

/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) += v;
        }
    }

    return ret;
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t tj = 0; tj < mat1.ncol(); tj += tsize)
    {
        for (size_t ti = 0; ti < mat1.nrow(); ti += tsize)
        {
            for (size_t tk = 0; tk < mat2.ncol(); tk += tsize)
            {
                for (size_t j = tj; j < std::min(tj + tsize, mat1.ncol()); ++j)
                {
                    for (size_t i = ti; i < std::min(ti + tsize, mat1.nrow()); ++i)
                    {
                        for (size_t k = tk; k < std::min(tk + tsize, mat2.ncol()); ++k)
                        {
                            ret(i,k) += mat1(i,j) * mat2(j,k);
                        }
                    }
                }
            }   
        }
    }

    return ret;    
}

/*
 * Use MKL for the matrix matrix multiplication.
 * https://www.intel.com/content/www/us/en/docs/onemkl/tutorial-c/2021-4/multiplying-matrices-using-dgemm.html
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2) 
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol(), std::vector<double>(mat1.nrow()*mat2.ncol(),0));

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.buffer(0) /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.buffer(0) /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.buffer(0) /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}
