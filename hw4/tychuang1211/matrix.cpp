#include "matrix.hpp"

template <class T>
T* CustomAllocator<T>::allocate( std::size_t n )
{
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
    {
        throw std::bad_alloc();
    }
    const std::size_t bytes = n*sizeof(T);
    T * p = static_cast<T *>(std::malloc(bytes));
    if (p)
    {
        m_allocated += bytes;
        return p;
    }
    else
    {
        throw std::bad_alloc();
    }
}

template <class T>
void CustomAllocator<T>::deallocate( T* p, std::size_t n ) noexcept
{
    std::free(p);

    const std::size_t bytes = n*sizeof(T);
    m_deallocated += bytes;
}

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
    m_buffer.clear();
    const size_t nelement = nrow * ncol;
    m_buffer.resize(nelement);
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
    
    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t ncol2 = mat2.ncol();
    
    Matrix ret(nrow1, ncol2);

    for (size_t tk = 0; tk < ncol2; tk += tsize)
    {
        size_t kmax = std::min(tk + tsize, ncol2)*ncol2;
        for (size_t ti = 0; ti < nrow1; ti += tsize)
        {
            size_t imax = std::min(ti + tsize, nrow1);
            for (size_t tj = 0; tj < ncol1; tj += tsize)
            {
                size_t jmax = std::min(tj + tsize, ncol1);
                for (size_t j = tj; j < jmax; ++j)
                {
                    for (size_t i = ti; i < imax; ++i)
                    {
                        for (size_t k = tk*ncol2; k < kmax; k+=ncol2)
                        {
                            ret.m_buffer[i+k] += mat1.m_buffer[i+j*ncol1] * mat2.m_buffer[j+k];
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
      , &(mat1.m_buffer[0]) /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , &(mat2.m_buffer[0]) /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , &(ret.m_buffer[0]) /* double * c */
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
