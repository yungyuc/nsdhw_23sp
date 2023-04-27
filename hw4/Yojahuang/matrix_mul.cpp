#include <stdexcept>
#include <cstddef>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>

namespace py = pybind11;

struct ByteCounterImpl
{

    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
    std::atomic_size_t refcount = 0;

}; /* end struct ByteCounterImpl */

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

    void incref() { ++m_impl->refcount; }

    void decref()
    {
        if (nullptr == m_impl)
        {
            // Do nothing.
        }
        else if (1 == m_impl->refcount)
        {
            delete m_impl;
            m_impl = nullptr;
        }
        else
        {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    static ByteCounter counter;

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

template <class T, class U>
bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return a.counter == b.counter;
}

template <class T, class U>
bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return !(a == b);
}

class Matrix
{
public:
    Matrix();
    ~Matrix();

    Matrix(size_t, size_t, std::vector<double> const);
    Matrix(size_t, size_t);

    double& operator()(size_t, size_t) const;
    bool operator==(const Matrix) const;
    bool operator!=(const Matrix) const;

    Matrix transpose() const;

    const size_t& n_row() const;
    const size_t& n_col() const;

    friend Matrix multiply_mkl(Matrix const&, Matrix const&);
private:
    // Member functions
    // Member data.
    size_t m_col, m_row;
    double* m_buffer;
}; /* end class Line */

class TileHelper
{
public:
    Matrix padMatrix(size_t, const Matrix&);
private:
}; /* end class Line */

Matrix TileHelper::padMatrix(size_t blocksize, const Matrix& originalMatrix) {
    size_t row, col;
    row = originalMatrix.n_row() + ((blocksize - (originalMatrix.n_row() % blocksize)) % blocksize);
    col = originalMatrix.n_col() + ((blocksize - (originalMatrix.n_col() % blocksize)) % blocksize);
    
    Matrix result(row, col);
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            if (i >= originalMatrix.n_row() || j >= originalMatrix.n_col()) result(i, j) = 0;
            else result(i,j) = originalMatrix(i, j);
        }
    }
    return result;
}

Matrix::Matrix() {
    this->m_row = 0;
    this->m_col = 0;
    this->m_buffer = NULL;
}

Matrix::~Matrix() {
    this->m_row = 0;
    this->m_col = 0;
}

Matrix::Matrix(size_t row, size_t col, std::vector<double> const value) {
    this->m_row = row;
    this->m_col = col;

    int cnt = 0;

    this->m_buffer = (double*)malloc(sizeof(double) * value.size());
    memset(this->m_buffer, 0, sizeof(double) * value.size());

    for (const double& v : value) {
        this->m_buffer[cnt++] = v;
    }
}

Matrix::Matrix(size_t row, size_t col) {
    this->m_row = row;
    this->m_col = col;

    size_t size = row * col;

    this->m_buffer = (double*)malloc(sizeof(double) * size);
    memset(this->m_buffer, 0, sizeof(double) * size);
}

Matrix Matrix::transpose() const {
    Matrix result(this->m_col, this->m_row);

    for (size_t i = 0; i < this->m_row; ++i) {
        for (size_t j = 0; j < this->m_col; ++j) {
            size_t idx = this->m_col * i + j;
            result(j, i) = this->m_buffer[idx];
        }
    }

    return result;
}

bool Matrix::operator==(const Matrix rhs) const {
    if (this->n_col() != rhs.n_col()) return false;
    if (this->n_row() != rhs.n_row()) return false;

    int col = this->m_col, row = this->m_row;

    for (int i = 0; i < col; ++i) {
        for (int j = 0; j < row; ++j) {
            size_t idx = this->m_col * i + j;
            double diff = abs(this->m_buffer[idx] - rhs(i,j));
            if (diff > 0.0000001) return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix rhs) const {
    return !(*this == rhs);
}


double& Matrix::operator()(size_t r, size_t c) const {
    size_t idx = r * this->m_col + c;
    return this->m_buffer[idx];
}

const size_t& Matrix::n_row() const {
    return this->m_row;
}

const size_t& Matrix::n_col() const {
    return this->m_col;
}

Matrix multiply_naive(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.n_col() != rhs.n_row()) {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    int r = lhs.n_row(), c = rhs.n_col(), n = lhs.n_col();
    
    Matrix result(r, c);

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            for (int k = 0; k < n; ++k) {
                result(i, j) = result(i, j) + lhs(i, k) * rhs(k, j);
            }
        }
    }
    return result;
}

Matrix multiply_tile(const Matrix& lhs, const  Matrix& rhs, size_t blocksize) {
    if (lhs.n_col() != rhs.n_row()) {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    TileHelper helper;

    // Padding 0 so that the row and col can be divisible by block size
    Matrix fakeLhs, fakeRhs;

    fakeLhs = helper.padMatrix(blocksize, lhs);
    fakeRhs = helper.padMatrix(blocksize, rhs);

    Matrix result(lhs.n_row(), rhs.n_col()), fakeResult(fakeLhs.n_row(), fakeRhs.n_col());

    size_t lhsRowOffset, lhsColOffset;
    size_t rhsRowOffset, rhsColOffset;

    for (lhsRowOffset = 0; lhsRowOffset < fakeLhs.n_row(); lhsRowOffset += blocksize) {
        for (rhsColOffset = 0; rhsColOffset < fakeRhs.n_col(); rhsColOffset += blocksize) {
            for (size_t cnt = 0; cnt < fakeLhs.n_col(); cnt += blocksize) {
                lhsColOffset = cnt;
                rhsRowOffset = cnt;

                // fakeResult[lhsRowId][rhsColId] += fakeLhs[lhsRowId][lhsColId] * fakeRhs[rhsRowId][rhsColId];

                // Parse matrix
                Matrix lhsInBlock(blocksize, blocksize), rhsInBlock(blocksize, blocksize);
                for (size_t i = 0; i < blocksize; ++i) {
                    for (size_t j = 0; j < blocksize; ++j) {
                        lhsInBlock(i, j) = fakeLhs(lhsRowOffset + i, lhsColOffset + j);
                    }
                }

                for (size_t i = 0; i < blocksize; ++i) {
                    for (size_t j = 0; j < blocksize; ++j) {
                        rhsInBlock(i, j) = fakeRhs(rhsRowOffset + i, rhsColOffset + j);
                    }
                }

                rhsInBlock = rhsInBlock.transpose();

                // Store back to result
                size_t resultRowOffset = lhsRowOffset, resultColOffset = rhsColOffset;

                for (size_t i = 0; i < blocksize; ++i) {
                    for (size_t j = 0; j < blocksize; ++j) {
                        double v = 0;
                        for (size_t k = 0; k < blocksize; ++k) {
                            v += lhsInBlock(i, k) * rhsInBlock(j, k);
                        }
                        fakeResult(resultRowOffset + i, resultColOffset + j) += v;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < result.n_row(); ++i) {
        for (size_t j = 0; j < result.n_col(); ++j) {
            result(i, j) = fakeResult(i, j);
        }
    }

    return result;
}


Matrix multiply_mkl(const Matrix& mat1, const Matrix& mat2)
{
    if (mat1.n_col() != mat2.n_row()) {
        throw std::invalid_argument("The shape of matrix is not align");
    }
    Matrix ret(mat1.n_row(), mat2.n_col());

    cblas_dgemm(
        CblasRowMajor
        , CblasNoTrans
        , CblasNoTrans
        , mat1.n_row()
        , mat2.n_col()
        , mat1.n_col()
        , 1.0
        , mat1.m_buffer
        , mat1.n_col()
        , mat2.m_buffer
        , mat2.n_col()
        , 0.0
        , ret.m_buffer
        , ret.n_col()
    );

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "Matrix multiplication function implementation";      // module doc string
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);
  py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>())
    .def_property_readonly("nrow", [](Matrix& m){ return m.n_row(); })
    .def_property_readonly("ncol", [](Matrix& m){ return m.n_col(); })
    .def(py::self == py::self) // eq
    .def(py::self != py::self) // neq
    .def("__getitem__", [](Matrix& m, std::pair<size_t, size_t> idx) { return m(idx.first, idx.second); })
    .def("__setitem__", [](Matrix& m, std::pair<size_t, size_t> idx, double val) { m(idx.first, idx.second) = val; });
}
