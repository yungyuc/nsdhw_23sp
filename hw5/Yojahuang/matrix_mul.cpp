#include <stdexcept>
#include <cstddef>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>

#include <limits>
#include <vector>

namespace py = pybind11;

struct _AllocatedCount
{
    std::size_t allocated;
    std::size_t deallocated;

    void increase(std::size_t sz)
    {
        this->allocated += sz;
    }

    void decrease(std::size_t sz)
    {
        this->deallocated += sz;
    }
};

template <class T>
class CustomAllocator
{
private:
    static struct _AllocatedCount count;

public:
    using value_type = T;
    static size_t bytes()
    {
        return count.allocated - count.deallocated;
    }
    static size_t allocated()
    {
        return count.allocated;
    }
    static size_t deallocated()
    {
        return count.deallocated;
    }
    CustomAllocator() noexcept = default;

    T *allocate(std::size_t sz)
    {
        if (sz > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t n_bytes = sz * sizeof(T);
        T *ptr = static_cast<T *>(std::malloc(n_bytes));
        if (ptr)
        {
            count.increase(n_bytes);
            return ptr;
        }
        throw std::bad_alloc();
    }
    void deallocate(T *ptr, std::size_t sz) noexcept
    {
        std::free(ptr);

        const std::size_t n_bytes = sz * sizeof(T);
        count.decrease(n_bytes);
    }
};

template <class T>
struct _AllocatedCount CustomAllocator<T>::count =
{
    .allocated = 0,
    .deallocated = 0
};

class Matrix
{
public:
    Matrix()
    {
    }
    Matrix(size_t c, size_t r)
    {
        m_col = c;
        m_row = r;
        m_buffer.resize(c * r);
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
    }

    ~Matrix()
    {
        m_col = 0;
        m_row = 0;
        m_buffer.clear();
    }

    const size_t &n_row() const
    {
        return m_row;
    }

    const size_t &n_col() const
    {
        return m_col;
    }

    double operator()(size_t r, size_t c) const
    {
        return m_buffer[r * m_col + c];
    }

    double &operator()(size_t r, size_t c)
    {
        return m_buffer[r * m_col + c];
    }

    double operator()(size_t idx) const
    {
        return m_buffer[idx];
    }

    double &operator()(size_t idx)
    {
        return m_buffer[idx];
    }

    bool operator==(const Matrix &rhs) const
    {
        if (m_col != rhs.n_col())
            return false;
        if (m_row != rhs.n_row())
            return false;

        for (size_t i = 0; i < m_row; ++i)
        {
            for (size_t j = 0; j < m_col; ++j)
            {
                if (m_buffer[i * m_col + j] != rhs(i, j))
                    return false;
            }
        }
        return true;
    }

    Matrix &operator=(const Matrix &rhs)
    {
        if (m_row != rhs.n_row() && m_col != rhs.n_col())
        {
            throw std::invalid_argument("Please don't tried to change the shape of the matrix");
        }
        size_t n = m_row * m_col;
        for (size_t i = 0; i < n; i++)
        {
            m_buffer[i] = rhs(i);
        }
        return *this;
    }
    size_t m_col, m_row;
    std::vector<double, CustomAllocator<double>> m_buffer;
};

Matrix &multiply_naive(Matrix &lhs, const Matrix &rhs)
{
    if (lhs.n_col() != rhs.n_row())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    size_t r = lhs.n_row(), c = rhs.n_col(), n = lhs.n_col();

    Matrix *result = new Matrix(r, c);

    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < c; ++j)
        {
            for (size_t k = 0; k < n; ++k)
            {
                (*result)(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return (*result);
}

void loadBlock(Matrix &m, double *buf, size_t r, size_t c, size_t blocksize)
{
    size_t rowStart = r * blocksize;
    size_t colStart = c * blocksize;

    size_t row, col;

    size_t idx = 0, cnt = 0;

    for (row = rowStart; row < std::min(rowStart + blocksize, m.n_row()); ++row)
    {
        for (col = colStart; col < std::min(colStart + blocksize, m.n_col()); ++col)
        {
            buf[idx] = m(rowStart, colStart);
            idx++;
        }
        cnt++;
        idx = cnt * blocksize;
    }
}

void mulBlock(double *lhsBuf, double *rhsBuf, size_t blocksize, double *resultBuf)
{
    for (size_t i = 0; i < blocksize; ++i)
    {
        for (size_t k = 0; k < blocksize; ++k)
        {
            for (size_t j = 0; j < blocksize; ++j)
            {
                resultBuf[i * blocksize + j] += lhsBuf[i * blocksize + k] * rhsBuf[k * blocksize + j];
            }
        }
    }
}

void saveBuf(double *resultBuf, Matrix *result, size_t blocksize, size_t r, size_t c)
{
    size_t rowStart = r * blocksize;
    size_t colStart = c * blocksize;

    size_t row, col;

    size_t idx = 0, cnt = 0;

    for (row = rowStart; row < std::min(rowStart + blocksize, (*result).n_row()); ++row)
    {
        for (col = colStart; col < std::min(colStart + blocksize, (*result).n_col()); ++col)
        {
            (*result)(row, col) = resultBuf[idx];
            idx++;
        }
        cnt++;
        idx = cnt * blocksize;
    }
}

Matrix &multiply_tile(Matrix &lhs, Matrix &rhs, size_t blocksize)
{
    if (lhs.n_col() != rhs.n_row())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    Matrix *result = new Matrix(lhs.n_row(), rhs.n_col());

    double lhsBuf[blocksize * blocksize];
    std::fill(lhsBuf, lhsBuf + blocksize * blocksize, 0);

    double rhsBuf[blocksize * blocksize];
    std::fill(rhsBuf, rhsBuf + blocksize * blocksize, 0);

    double resultBuf[blocksize * blocksize];
    std::fill(resultBuf, resultBuf + blocksize * blocksize, 0);

    size_t lhsRow = lhs.n_row() / blocksize + (lhs.n_row() % blocksize > 0);
    size_t lhsCol = lhs.n_col() / blocksize + (lhs.n_col() % blocksize > 0);

    size_t rhsCol = rhs.n_col() / blocksize + (rhs.n_col() % blocksize > 0);

    for (size_t i = 0; i < lhsRow; ++i)
    {
        for (size_t j = 0; j < rhsCol; ++j)
        {
            for (size_t k = 0; k < lhsCol; ++k)
            {
                loadBlock(lhs, lhsBuf, i, k, blocksize);
                loadBlock(rhs, rhsBuf, k, j, blocksize);
                mulBlock(lhsBuf, rhsBuf, blocksize, resultBuf);
                saveBuf(resultBuf, result, blocksize, i, j);
            }
        }
    }

    return *result;
}

Matrix &multiply_mkl(Matrix &mat1, Matrix &mat2)
{
    if (mat1.n_col() != mat2.n_row())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }
    Matrix *ret = new Matrix(mat1.n_row(), mat2.n_col());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.m_row,
        mat2.m_col,
        mat1.m_col,
        1.0,
        mat1.m_buffer.data(),
        mat1.m_col,
        mat2.m_buffer.data(),
        mat2.m_col,
        0.0,
        ret->m_buffer.data(),
        ret->m_col);

    return *ret;
}