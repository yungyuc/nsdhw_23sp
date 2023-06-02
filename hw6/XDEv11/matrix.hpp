#include <mkl/mkl.h>

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <cassert>

class Matrix {
	using element_t = double;
	size_t n, m;
	std::vector<element_t> mtx;
public:
	explicit Matrix(size_t nrow, size_t ncol) : n{nrow}, m{ncol}, mtx(n * m) {}
	size_t nrow() const { return n; }
	size_t ncol() const { return m; }
	element_t& operator[](std::array<size_t, 2> idx) { return mtx[idx[0] * ncol() + idx[1]]; }
	element_t& operator()(size_t r, size_t c) { return mtx[r * ncol() + c]; }
	const element_t& operator()(size_t r, size_t c) const { return mtx[r * ncol() + c]; }
	double* data() { return mtx.data(); }
	const double* data() const { return mtx.data(); }
	bool operator==(const Matrix& other) const { return mtx == other.mtx; }
	bool operator!=(const Matrix& other) const { return mtx != other.mtx; }
	std::string str() const {
		std::string s{};
		for (size_t i{0}; i < nrow(); ++i) {
			s += (i == 0 ? "[[" : " [");
			for (size_t j{0}; j < ncol(); ++j) s += std::to_string(mtx[i * ncol() + j]) + (j == ncol() - 1 ? "" : ", ");
			s += (i == nrow() - 1 ? "]]" : "]\n");
		}
		return s;
	}
};

void MYMAC1(const Matrix& m1, std::array<size_t, 2> s, Matrix& m2) {
	assert("MYMAC1()");
	for (size_t i{0}; i < m2.nrow(); ++i)
		for (size_t j{0}; j < m2.ncol(); ++j) m2(i, j) += m1(i + s[0], j + s[1]);
}

void MYMAC2(const Matrix& m1, Matrix& m2, std::array<size_t, 2> s) {
	assert("MYMAC2()");
	for (size_t i{0}; i < m1.nrow(); ++i)
		for (size_t j{0}; j < m1.ncol(); ++j) m2(i + s[0], j + s[1]) += m1(i, j);
}

void MYDGMMMAC(const Matrix& m1, const Matrix& m2, Matrix& m3) {
	assert(m1.ncol() == m2.nrow() && m1.nrow() == m3.nrow() && m2.ncol() == m3.ncol() && "MYDGMMMAC()");
	for (size_t i{0}; i < m1.nrow(); ++i)
		for (size_t j{0}; j < m2.ncol(); ++j)
			for (size_t k{0}; k < m1.ncol(); ++k)
				m3(i, j) += m1(i, k) * m2(k, j);
}

Matrix multiply_naive(const Matrix& m1, const Matrix& m2) {
	assert(m1.ncol() == m2.nrow() && "multiply_naive()");
	Matrix m3{m1.nrow(), m2.ncol()};
	MYDGMMMAC(m1, m2, m3);
	return m3;
}

Matrix multiply_tile(const Matrix& m1, const Matrix& m2, const size_t& ts) {
	assert(m1.ncol() == m2.nrow() && "multiply_tile()");
	Matrix m3{m1.nrow(), m2.ncol()};
	for (size_t i{0}; i < m1.nrow(); i += ts)
		for (size_t j{0}; j < m2.ncol(); j += ts) {
			Matrix C{std::min(m1.nrow() - i, ts), std::min(m2.ncol() - j, ts)};
			for (size_t k{0}; k < m1.ncol(); k += ts) {
				Matrix A{std::min(m1.nrow() - i, ts), std::min(m1.ncol() - k, ts)}; MYMAC1(m1, {i, k}, A);
				Matrix B{std::min(m1.ncol() - k, ts), std::min(m2.ncol() - j, ts)}; MYMAC1(m2, {k, j}, B);
				MYDGMMMAC(A, B, C);
			}
			MYMAC2(C, m3, {i, j});
		}
	return m3;
}

Matrix multiply_mkl(const Matrix& m1, const Matrix& m2) {
	Matrix m3{m1.nrow(), m2.ncol()};

	cblas_dgemm(
			CblasRowMajor,	/* const CBLAS_LAYOUT Layout */
			CblasNoTrans,	/* const CBLAS_TRANSPOSE transa */
			CblasNoTrans,	/* const CBLAS_TRANSPOSE transb */
			m1.nrow(),			/* const MKL_INT m */
			m2.ncol(),			/* const MKL_INT n */
			m1.ncol(),			/* const MKL_INT k */
			1.0,			/* const double alpha */
			m1.data(),		/* const double *a */
			m1.ncol(),			/* const MKL_INT lda */
			m2.data(),		/* const double *b */
			m2.ncol(),			/* const MKL_INT ldb */
			0.0,			/* const double beta */
			m3.data(),		/* double * c */
			m3.ncol()			/* const MKL_INT ldc */
	);

	return m3;
}
