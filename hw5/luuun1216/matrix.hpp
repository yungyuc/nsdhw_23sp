#include <mkl.h>

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


struct Matrix {
    ~Matrix() { reset_buffer(0, 0); }

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
        : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix(Matrix const &other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    Matrix &operator=(std::vector<double> const &vec) {
        if (size() != vec.size()) {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix &operator=(Matrix const &other) {
        if (this == &other) {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = other(i, j);
            }
        }
        return *this;
    }

    Matrix &operator=(Matrix &&other) {
        if (this == &other) {
            return *this;
        }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    Matrix &operator=(double val) {
        for (size_t i = 0, end = m_nrow * m_ncol; i < end; ++i) {
            m_buffer[i] = val;
        }
        return *this;
    }

    Matrix &operator+=(const Matrix &other) {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            throw std::out_of_range("the shape of first matrix differs from"
                                    "that of second matrix");
        }

        for (size_t i = 0, end = m_nrow * m_ncol; i < end; ++i) {
            m_buffer[i] += other.m_buffer[i];
        }
        return *this;
    }

    double operator()(size_t row, size_t col) const {
        return m_buffer[index(row, col)];
    }
    double &operator()(size_t row, size_t col) {
        return m_buffer[index(row, col)];
    }

    double operator[](size_t idx) const { return m_buffer[idx]; }
    double &operator[](size_t idx) { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const {
        return std::vector<double>(m_buffer, m_buffer + size());
    }

    Matrix transpose() const;

    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

    void save(Matrix &mat, size_t it, size_t jt);

    void reset_buffer(size_t nrow, size_t ncol) {
        if (m_buffer) {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement) {
            m_buffer = new double[nelement];
        } else {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, const size_t tile_size);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    friend bool operator==(Matrix const &mat1, Matrix const &mat2);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};

Matrix Matrix::transpose() const {
    Matrix ret(nrow(), ncol());

    for (size_t i = 0; i < ret.nrow(); ++i) {
        for (size_t j = 0; j < ret.ncol(); ++j) {
            ret(j, i) = (*this)(i, j);
        }
    }

    return ret;
}

bool operator==(Matrix const &mat1, Matrix const &mat2) {
    if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol())) {
        return false;
    }

    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t j = 0; j < mat1.ncol(); ++j) {
            if (mat1(i, j) != mat2(i, j)) {
                return false;
            }
        }
    }

    return true;
}

bool operator!=(Matrix const &mat1, Matrix const &mat2) {
    return !(mat1 == mat2);
}

void Matrix::save(Matrix &mat, size_t tile_row, size_t tile_col) {
    const size_t ncol = mat.ncol();

    for (size_t i = 0; i < m_nrow; ++i) {
        const size_t start_idx_of_block = i * m_nrow;
        const size_t elements_of_stride_blocks_in_row =
            tile_row * (m_nrow * ncol);
        const size_t elements_of_stride_matrix = i * ncol;
        const size_t elements_of_stride_blocks_in_col = tile_col * m_ncol;
        const size_t start_idx_of_matrix = elements_of_stride_blocks_in_row +
                                           elements_of_stride_matrix +
                                           elements_of_stride_blocks_in_col;
        for (size_t j = 0; j < m_ncol; ++j) {
            mat.m_buffer[start_idx_of_matrix + j] =
                m_buffer[start_idx_of_block + j];
        }
    }
}

/*
 * Throw an exception if the shapes of the two matrices don't support
 * multiplication.
 */
void validate_multiplication(Matrix const &mat1, Matrix const &mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("the number of first matrix column "
                                "differs from that of second matrix row");
    }
}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2) {
    validate_multiplication(mat1, mat2);

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transa */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transb */
                ,
                mat1.nrow() /* const MKL_INT m */
                ,
                mat2.ncol() /* const MKL_INT n */
                ,
                mat1.ncol() /* const MKL_INT k */
                ,
                1.0 /* const double alpha */
                ,
                mat1.m_buffer /* const double *a */
                ,
                mat1.ncol() /* const MKL_INT lda */
                ,
                mat2.m_buffer /* const double *b */
                ,
                mat2.ncol() /* const MKL_INT ldb */
                ,
                0.0 /* const double beta */
                ,
                ret.m_buffer /* double * c */
                ,
                ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2) {
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t k = 0; k < mat2.ncol(); ++k) {
            double result = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j) {
                result += mat1(i, j) * mat2(j, k);
            }

            ret(i, k) = result;
        }
    }

    return ret;
}

struct Tiler {
    Tiler(size_t n) : m_row_n_col(n), m_mat1(n, n), m_mat2(n, n), m_ret(n, n) {}

    void load_tile(const Matrix &mat, const size_t tile_row,
                   const size_t tile_col);
    void load(Matrix const &mat1, size_t it1, size_t jt1, Matrix const &mat2,
              size_t it2, size_t jt2);

    void multiply();

    size_t m_row_n_col;
    Matrix m_mat1; // row-major
    Matrix m_mat2; // col-major
    Matrix m_ret;  // row-major
};

static void load_tile_helper(const size_t row_n_col, Matrix &mat1, const Matrix &mat2,
                             const size_t tile_row, const size_t tile_col) {
    for (size_t i = 0; i < row_n_col; ++i) {
        const size_t start_idx_of_block = i * row_n_col;
        const size_t elements_of_stride_blocks_in_row =
            tile_row * (row_n_col * mat2.ncol());
        const size_t elements_of_stride_matrix = i * mat2.ncol();
        const size_t elements_of_stride_blocks_in_col = tile_col * row_n_col;
        const size_t start_idx_of_matrix = elements_of_stride_blocks_in_row +
                                           elements_of_stride_matrix +
                                           elements_of_stride_blocks_in_col;
        for (size_t j = 0; j < row_n_col; ++j) {
            mat1[start_idx_of_block + j] =
                mat2.m_buffer[start_idx_of_matrix + j];
        }
    }
}

void Tiler::load(const Matrix &mat1, size_t it1, size_t jt1, const Matrix &mat2,
                 size_t it2, size_t jt2) {

    // row-major
    load_tile_helper(m_row_n_col, m_mat1, mat1, it1, jt1);

    load_tile_helper(m_row_n_col, m_ret, mat2, it2, jt2);

    // col-major
    for (size_t i = 0; i < m_row_n_col; ++i) {
        const size_t start_idx_of_block = i * m_row_n_col;

        for (size_t j = 0; j < m_row_n_col; ++j) {
            m_mat2[j * m_row_n_col + i] = m_ret[start_idx_of_block + j];
        }
    }
}

void Tiler::multiply() {
    for (size_t i = 0; i < m_row_n_col; ++i) {
        const size_t base1 = i * m_row_n_col;

        for (size_t k = 0; k < m_row_n_col; ++k) {
            const size_t base2 = k * m_row_n_col;

            double val = 0;
            for (size_t j = 0; j < m_row_n_col; ++j) {
                val += m_mat1[base1 + j] * m_mat2[base2 + j];
            }
            m_ret[base1 + k] = val;
        }
    }
}

// tile_size: number of elements
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, const size_t tile_size) {
    validate_multiplication(mat1, mat2);

    if (tile_size == 0 || tile_size > mat1.nrow()) {
        return multiply_naive(mat1, mat2);
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    ret = 0;

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = nrow1 / tile_size;
    const size_t ntcol1 = ncol1 / tile_size;
    const size_t ntrow2 = nrow2 / tile_size;
    const size_t ntcol2 = ncol2 / tile_size;

	Matrix value(tile_size, tile_size);
    Tiler tiler(tile_size);

    for (size_t it = 0; it < ntrow1; ++it) {
        for (size_t kt = 0; kt < ntcol2; ++kt) {
            value = 0;
            for (size_t jt = 0; jt < ntcol1; ++jt) {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.m_ret;
            }
            value.save(ret, it, kt);
        }
    }

    // handle elements in tail of a row
    const size_t remain_ncol1 = ncol1 % tile_size;
    const size_t remain_ncol2 = ncol2 % tile_size;
    if (remain_ncol1 != 0) {
        for (size_t i = 0; i < nrow1; ++i) {
            for (size_t k = 0, end = ncol2 - remain_ncol2; k < end; ++k) {
                double result = 0;
                for (size_t j = ncol1 - remain_ncol1; j < ncol1; ++j) {
                    result += mat1(i, j) * mat2(j, k);
                }

                ret(i, k) += result;
            }

            if (ncol1 > 8) {
                for (size_t k = ncol2 - remain_ncol2; k < ncol2; ++k) {
                    double result = 0;
                    size_t j = 0;
                    for (; (j + 8) < ncol1; j += 8) {
                        result += mat1(i, j) * mat2(j, k);
                        result += mat1(i, j + 1) * mat2(j + 1, k);
                        result += mat1(i, j + 2) * mat2(j + 2, k);
                        result += mat1(i, j + 3) * mat2(j + 3, k);
                        result += mat1(i, j + 4) * mat2(j + 4, k);
                        result += mat1(i, j + 5) * mat2(j + 5, k);
                        result += mat1(i, j + 6) * mat2(j + 6, k);
                        result += mat1(i, j + 7) * mat2(j + 7, k);
                    }

                    if (j != ncol1) {
                        for (; j < ncol1; ++j) {
                            result += mat1(i, j) * mat2(j, k);
                        }
                    }

                    ret(i, k) += result;
                }
            } else {
                for (size_t k = ncol2 - remain_ncol2; k < ncol2; ++k) {
                    double result = 0;
                    for (size_t j = 0; j < ncol1; ++j) {
                        result += mat1(i, j) * mat2(j, k);
                    }

                    ret(i, k) += result;
                }
            }
        }
    }

    // handle whole unhandled row
    const size_t remain_nrow1 = nrow1 % tile_size;
    if (remain_nrow1 != 0) {
        for (size_t i = nrow1 - remain_nrow1; i < nrow1; ++i) {
            size_t j_end = ncol1 - remain_ncol1;
            if (j_end > 8) {
                for (size_t k = 0, end = ncol2 - remain_ncol2; k < end; ++k) {
                    double result = 0;
                    size_t j = 0;
                    for (; (j + 8) < j_end; j += 8) {
                        result += mat1(i, j) * mat2(j, k);
                        result += mat1(i, j + 1) * mat2(j + 1, k);
                        result += mat1(i, j + 2) * mat2(j + 2, k);
                        result += mat1(i, j + 3) * mat2(j + 3, k);
                        result += mat1(i, j + 4) * mat2(j + 4, k);
                        result += mat1(i, j + 5) * mat2(j + 5, k);
                        result += mat1(i, j + 6) * mat2(j + 6, k);
                        result += mat1(i, j + 7) * mat2(j + 7, k);
                    }

                    if (j != j_end) {
                        for (; j < j_end; ++j) {
                            result += mat1(i, j) * mat2(j, k);
                        }
                    }

                    ret(i, k) += result;
                }
            } else {
                for (size_t k = 0, end = ncol2 - remain_ncol2; k < end; ++k) {
                    double result = 0;
                    for (size_t j = 0; j < j_end; ++j) {
                        result += mat1(i, j) * mat2(j, k);
                    }

                    ret(i, k) += result;
                }
            }
        }
    }

    return ret;
}