#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sstream>
#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>

// #define BLK_NUM 32
static size_t BLK_NUM = 32;

namespace py=pybind11;

class Matrix {
    public:
        size_t nrow;
        size_t ncol;
        double *buf;
        size_t n_elem;
        Matrix(size_t n_row, size_t n_col) {
            if(n_row == 0 || n_col == 0) {
                throw std::invalid_argument("number of rows or columns should not be zero");
            }
            this->nrow = n_row;
            this->ncol = n_col;
            this->n_elem = n_row * n_col;
            buf = new double[this->n_elem];
            std::fill(buf, buf + n_elem, (double)0);
        }
        double operator() (size_t row, size_t col) const {
            return buf[row * ncol + col];
        }
        double & operator()(size_t row, size_t col) {
            return buf[row * ncol + col];
        }

        bool operator==(const Matrix &ano) const{
            if(ano.nrow != this->nrow || ano.ncol != this->ncol) {
                return false;
            }
            for(size_t i = 0; i < ano.nrow; i ++) {
                for(size_t j = 0; j < ano.ncol; j ++) {
                    if(ano(i, j) != (*this)(i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }

        Matrix & operator=(const Matrix &ano) {
            if(this->nrow != ano.nrow && this->ncol != ano.ncol) {
                throw std::invalid_argument("Cannot assign matrix to different shape");
            }
            size_t n_elem = this->nrow * this->ncol;
            for(size_t i = 0; i < n_elem; i ++) {
                this->buf[i] = ano[i];
            }
            return *this;
        }

        double operator[] (size_t offset) const {
            return buf[offset];
        }
        double & operator[](size_t offset) {
            return buf[offset];
        }

};

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    if(A.ncol != B.nrow) {
        throw std::invalid_argument("A's cols should be the same as B's rows");
    }
    Matrix ret = Matrix(A.nrow, B.ncol);
    for(size_t i = 0; i < ret.nrow; i ++) {
        for(size_t j = 0; j < ret.ncol; j ++) {
            double v = 0;
            for(size_t k = 0; k < A.ncol; k ++) {
                v += A(i, k) * B(k, j);
            }
            (ret)(i, j) = v;
        }
    }
    return ret;
}

static void load_buf(double retbuf[], const Matrix &A, double Abuf[], size_t it1, size_t jt1, const Matrix &B, double Bbuf[], size_t it2, size_t jt2) {
    const size_t ncol1 = A.ncol;
    for(size_t i = 0; i < BLK_NUM; i ++) {
        size_t base_t = i * BLK_NUM;
        size_t base_i = (it1 * BLK_NUM + i);
        size_t base_j = jt1 * BLK_NUM;
        if(base_i >= A.nrow) {
            break;
        }
        base_i *= ncol1;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            if(base_j + j < A.ncol)
                Abuf[base_t + j] = A[base_i + base_j + j];
            else
                Abuf[base_t + j] = (double)0;
        }
    }
    const size_t ncol2 = B.ncol;
    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base_t = i * BLK_NUM;
        size_t base_i = (it2 * BLK_NUM + i);
        size_t base_j = (jt2) * BLK_NUM;
        if(base_i >= B.nrow) {
            break;
        }
        base_i *= ncol2;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            if(base_j + j < B.ncol)
                retbuf[base_t + j] = B[base_i + base_j + j];
            else
                retbuf[base_t + j] = (double)0;
        }
    }

    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base = i * BLK_NUM;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            Bbuf[j * BLK_NUM + i] = retbuf[base + j];
        }
    }
}

static void tile_mul(double retbuf[], double Abuf[], double Bbuf[]) {

    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base1 = i * BLK_NUM;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            const size_t base2 = j * BLK_NUM;
            double v = 0;
            for(size_t k = 0; k < BLK_NUM; k ++) {
                v += Abuf[base1 + k] * Bbuf[base2 + k];
            }
            retbuf[base1 + j] = v;
        }
    }
}

static void save(Matrix &ret, double retbuf[], size_t it, size_t jt) {
    const size_t ncol = ret.ncol;
    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base_s = i * BLK_NUM;
        const size_t base_t = (it * BLK_NUM + i) * ncol + jt * BLK_NUM;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            if(base_t + j < ret.n_elem)
                ret[base_t + j] += retbuf[base_s + j];
        }
    }
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_size) {
    if(A.ncol != B.nrow) {
        throw std::invalid_argument("A's cols should be the same as B's rows");
    }
    if(tile_size == 0) {
        throw std::invalid_argument("tile_size should greater than 0");
    }
    BLK_NUM = tile_size;

    Matrix ret = Matrix(A.nrow, B.ncol);
    double Abuf[BLK_NUM * BLK_NUM];
    std::fill(Abuf, Abuf + BLK_NUM * BLK_NUM, (double)0);
    double Bbuf[BLK_NUM * BLK_NUM];
    std::fill(Bbuf, Bbuf + BLK_NUM * BLK_NUM, (double)0);
    double retbuf[BLK_NUM * BLK_NUM];
    std::fill(retbuf, retbuf + BLK_NUM * BLK_NUM, (double)0);
    size_t row1 = A.nrow / BLK_NUM + (A.nrow % BLK_NUM > 0);
    size_t col1 = A.ncol / BLK_NUM + (A.nrow % BLK_NUM > 0);
    size_t row2 = B.nrow / BLK_NUM + (B.nrow % BLK_NUM > 0);
    size_t col2 = B.ncol / BLK_NUM + (B.nrow % BLK_NUM > 0);
    for(size_t i = 0; i < row1; i ++) {
        for(size_t j = 0; j < col2; j ++) {
            for(size_t k = 0; k < col1; k ++) {
                load_buf(retbuf, A, Abuf, i, k, B, Bbuf, k, j);
                tile_mul(retbuf, Abuf, Bbuf);
                save(ret, retbuf, i, j);
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix &A, Matrix &B) {
    Matrix ret(A.nrow, B.ncol);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , A.nrow /* const MKL_INT m */
      , B.ncol /* const MKL_INT n */
      , A.ncol /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , A.buf /* const double *a */
      , A.ncol /* const MKL_INT lda */
      , B.buf /* const double *b */
      , B.ncol /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.buf /* double * c */
      , ret.ncol /* const MKL_INT ldc */
    );

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__getitem__", [](const Matrix &self, const size_t elem) {
            return self[elem];
        })
        .def("__getitem__", [](const Matrix &self, const std::pair<size_t, size_t> p) {
            return self(p.first, p.second);
        })
        .def("__setitem__", [](Matrix &self, const size_t elem, double val) {
            self[elem] = val;
        })
        .def("__setitem__", [](Matrix &self, const std::pair<size_t, size_t> p, double val) {
            self(p.first, p.second) = val;
        })
        .def("__repr__", [](const Matrix &self) {
            size_t ncol = self.ncol;
            size_t nrow = self.nrow;
            std::stringstream ss;
            for(size_t i = 0; i < nrow; i ++) {
                for(size_t j = 0; j < ncol; j ++) {
                    ss << self(i, j) << ' ';
                }
                if(i < nrow - 1)
                    ss << '\n';
            }
            return ss.str();
        })
        .def(py::self == py::self)
        .def_readonly("nrow", &Matrix::nrow)
        .def_readonly("ncol", &Matrix::ncol);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}