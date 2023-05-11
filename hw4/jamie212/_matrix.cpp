#include <iostream>
#include <chrono>
#include <mkl.h>
#include <vector>
#include <atomic>
#include <limits>
#include <cstdlib>
#include <new>
#include <memory>
#include <pybind11/pybind11.h>

using namespace std;
using namespace chrono;
namespace py = pybind11;

# define RANDMAX 10

template <class T>
class CustomAllocator {

    public:
        using value_type = T;
        CustomAllocator() = default;

        T* allocate(size_t n)
        {
            if (n > std::numeric_limits<size_t>::max()/sizeof(T))
                throw std::bad_alloc();

            const size_t bytes = n*sizeof(T);
            T* ret = static_cast<T*>(malloc(bytes));

            if (ret)
            {
                m_allocated += bytes;
                return ret;
            }
            else
                throw std::bad_alloc();
        }

        void deallocate(T* p, size_t n) noexcept
        {
            free(p);
            m_deallocated += n*sizeof(T);
        }

        bool operator==(const CustomAllocator& other) const noexcept {
            return this == &other;
        }

        bool operator!=(const CustomAllocator& other) const noexcept {
            return !(*this == other);
        }

        static size_t allocated(){
            return m_allocated;
        }
        static size_t deallocated(){
            return m_deallocated;
        }
        static size_t bytes(){
            return m_allocated - m_deallocated;
        }

    private:
        static size_t m_allocated, m_deallocated;
};


class Matrix {
public:
    Matrix(): m_nrow(0), m_ncol(0), m_data(vector<double, CustomAllocator<double>>(0)) {}
    Matrix(int nrow, int ncol): m_nrow(nrow), m_ncol(ncol), m_data(vector<double, CustomAllocator<double>>(nrow * ncol)) {}
    double& operator()(int i, int j) {
        return m_data[i*m_ncol+j];
    }
    const double& operator()(int i, int j) const {
        return m_data[i*m_ncol+j];
    }

    bool operator == (Matrix const &matrix) const {
        for (size_t i = 0; i < m_nrow; i++) 
            for (size_t j = 0; j < m_ncol; j++)
                if (m_data[i*m_ncol + j] != matrix(i, j)) 
                    return false;
        return true;
    }

    double& operator()(size_t row, size_t col)
    {
        return m_data[row * m_ncol + col];
    }

    double operator()(size_t row, size_t col) const
    {
        return m_data[row * m_ncol + col];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double* data() { 
        return m_data.data(); 
    }
    const double* data() const { 
        return m_data.data(); 
    }

private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, CustomAllocator<double>> m_data;
};

template <class T> size_t CustomAllocator<T>::m_allocated = 0;
template <class T> size_t CustomAllocator<T>::m_deallocated = 0;

void generateValue(Matrix& A){
    size_t n = A.nrow();
    size_t m = A.ncol();
    for (size_t i = 0 ; i < n ; ++i){
        for (size_t j = 0 ; j < m ; ++j){
            double value = rand() % RANDMAX;
            A(i, j) = value;
        }
    }
}

Matrix multiply_naive(Matrix const & A, Matrix const & B)
{
    assert(A.ncol() == B.nrow());

    size_t n = A.nrow();
    size_t m = A.ncol();
    size_t p = B.ncol();

    Matrix C(n, p);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < p; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < m; k++) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

Matrix multiply_tile(Matrix& A, Matrix& B, size_t tile_size) {
    assert(A.ncol() == B.nrow());

    size_t n = A.nrow();
    size_t m = A.ncol();
    size_t p = B.ncol();

    Matrix C(n, p);

    for (size_t i = 0; i < C.nrow(); ++i) {
        for (size_t j = 0; j < C.ncol(); ++j) {
            C(i, j) = 0.0;
        }
    }

    for (size_t i = 0; i < n; i += tile_size) {
        for (size_t j = 0; j < p; j += tile_size) {
            for (size_t k = 0; k < m; k += tile_size) {
                for (size_t ii = i; ii < std::min(i + tile_size, n); ++ii) {
                    for (size_t jj = j; jj < std::min(j + tile_size, p); ++jj) {
                        double sum = 0.0;
                        for (size_t kk = k; kk < std::min(k + tile_size, m); ++kk) {
                            sum += A(ii, kk) * B(kk, jj);
                        }
                        C(ii, jj) += sum;
                    }
                }
            }
        }
    }
    return C;
}

Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    assert(A.ncol() == B.nrow());

    size_t n = A.nrow();
    size_t m = A.ncol();
    size_t p = B.ncol();

    Matrix E(n, p);

    const double alpha = 1.0;
    const double beta = 0.0;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, p, m, alpha, A.data(), m, B.data(), p, beta, E.data(), p);
    return E;
}


PYBIND11_MODULE(_matrix, m)
{
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<const Matrix &>())
        .def_property_readonly("nrow", [](const Matrix &mat){ return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat){ return mat.ncol(); })
        .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
        .def("__getitem__",
             [](const Matrix &m, std::pair<size_t, size_t> idx) {
                 return m(idx.first, idx.second);
             })
        .def("__setitem__",
             [](Matrix &m, std::pair<size_t, size_t> idx, double value) {
                 m(idx.first, idx.second) = value;
             });
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("generateValue", &generateValue);
    m.def("bytes", &CustomAllocator<double>::bytes);
    m.def("allocated", &CustomAllocator<double>::allocated);
    m.def("deallocated", &CustomAllocator<double>::deallocated);
}