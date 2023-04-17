#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <mkl/mkl.h>

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <cassert>
#include <memory>

template<typename T>
class CustomAllocator {
	using allocator_type = typename std::allocator<T>;
public:
	using value_type = typename allocator_type::value_type;
	using size_type = typename allocator_type::size_type;
private:
	allocator_type alctr;
	static size_type _bytes;
	static size_type _allocated;
	static size_type _deallocated;
public:
	CustomAllocator() noexcept : alctr{} {}
	value_type* allocate(size_type n, const void* hint = 0) {
		_allocated += n, _bytes += n;
		return alctr.allocate(n, hint);
	}
	void deallocate(value_type* p, size_type n) {
		_deallocated += n, _bytes -= n;
		alctr.deallocate(p, n);
	}
	static size_type bytes() { return _bytes * sizeof(value_type); }
	static size_type allocated() { return _allocated * sizeof(value_type); }
	static size_type deallocated() { return _deallocated * sizeof(value_type); }
};
template<typename T>
typename CustomAllocator<T>::size_type CustomAllocator<T>::_bytes{0};
template<typename T>
typename CustomAllocator<T>::size_type CustomAllocator<T>::_allocated{0};
template<typename T>
typename CustomAllocator<T>::size_type CustomAllocator<T>::_deallocated{0};

class Matrix {
	using element_t = double;
	size_t _n, _m;
	std::vector<element_t, CustomAllocator<double>> mtx;
public:
	explicit Matrix(size_t n, size_t m) : _n{n}, _m{m}, mtx(_n * _m) {}
	Matrix(const Matrix& other)=delete;
	Matrix(Matrix&& other)=default;
	Matrix& operator=(const Matrix& other)=delete;
	Matrix& operator=(Matrix&& other)=default;
	size_t n() const { return _n; }
	size_t m() const { return _m; }
	element_t& operator[](std::array<size_t, 2> idx) { return mtx[idx[0] * m() + idx[1]]; }
	element_t& operator()(size_t r, size_t c) { return mtx[r * m() + c]; }
	const element_t& operator()(size_t r, size_t c) const { return mtx[r * m() + c]; }
	double* data() { return mtx.data(); }
	const double* data() const { return mtx.data(); }
	bool operator==(const Matrix& other) { return mtx == other.mtx; }
	bool operator!=(const Matrix& other) { return mtx != other.mtx; }
	std::string str() {
		std::string s{};
		for (size_t i{0}; i < n(); ++i) {
			s += (i == 0 ? "[[" : " [");
			for (size_t j{0}; j < m(); ++j) s += std::to_string(mtx[i * m() + j]) + (j == m() - 1 ? "" : ", ");
			s += (i == n() - 1 ? "]]" : "]\n");
		}
		return s;
	}
};

void MYMAC1(const Matrix& m1, std::array<size_t, 2> s, Matrix& m2) {
	assert("MYMAC1()");
	for (size_t i{0}; i < m2.n(); ++i)
		for (size_t j{0}; j < m2.m(); ++j) m2(i, j) += m1(i + s[0], j + s[1]);
}

void MYMAC2(const Matrix& m1, Matrix& m2, std::array<size_t, 2> s) {
	assert("MYMAC2()");
	for (size_t i{0}; i < m1.n(); ++i)
		for (size_t j{0}; j < m1.m(); ++j) m2(i + s[0], j + s[1]) += m1(i, j);
}

void MYDGMMMAC(const Matrix& m1, const Matrix& m2, Matrix& m3) {
	assert(m1.m() == m2.n() && m1.n() == m3.n() && m2.m() == m3.m() && "MYDGMMMAC()");
	for (size_t i{0}; i < m1.n(); ++i)
		for (size_t j{0}; j < m2.m(); ++j)
			for (size_t k{0}; k < m1.m(); ++k)
				m3(i, j) += m1(i, k) * m2(k, j);
}

void MYDGMMMAC_better_order(const Matrix& m1, const Matrix& m2, Matrix& m3) {
	assert(m1.m() == m2.n() && m1.n() == m3.n() && m2.m() == m3.m() && "MYDGMMMAC_better_order()");
	for (size_t i{0}; i < m1.n(); ++i)
		for (size_t k{0}; k < m1.m(); ++k)
			for (size_t j{0}; j < m2.m(); ++j)
				m3(i, j) += m1(i, k) * m2(k, j);
}

Matrix multiply_naive(const Matrix& m1, const Matrix& m2) {
	assert(m1.m() == m2.n() && "multiply_naive()");
	Matrix m3{m1.n(), m2.m()};
	MYDGMMMAC(m1, m2, m3);
	return m3;
}

Matrix multiply_naive_better_order(const Matrix& m1, const Matrix& m2) {
	assert(m1.m() == m2.n() && "multiply_naive_better_order()");
	Matrix m3{m1.n(), m2.m()};
	MYDGMMMAC_better_order(m1, m2, m3);
	return m3;
}

Matrix multiply_tile(const Matrix& m1, const Matrix& m2, const size_t& ts) {
	assert(m1.m() == m2.n() && "multiply_tile()");
	Matrix m3{m1.n(), m2.m()};
	for (size_t i{0}; i < m1.n(); i += ts)
		for (size_t j{0}; j < m2.m(); j += ts) {
			Matrix C{std::min(m1.n() - i, ts), std::min(m2.m() - j, ts)};
			for (size_t k{0}; k < m1.m(); k += ts) {
				Matrix A{std::min(m1.n() - i, ts), std::min(m1.m() - k, ts)}; MYMAC1(m1, {i, k}, A);
				Matrix B{std::min(m1.m() - k, ts), std::min(m2.m() - j, ts)}; MYMAC1(m2, {k, j}, B);
				MYDGMMMAC(A, B, C);
			}
			MYMAC2(C, m3, {i, j});
		}
	return m3;
}

Matrix multiply_tile_better_order(const Matrix& m1, const Matrix& m2, const size_t& ts) {
	assert(m1.m() == m2.n() && "multiply_tile_better_order()");
	Matrix m3{m1.n(), m2.m()};
	for (size_t i{0}; i < m1.n(); i += ts)
		for (size_t k{0}; k < m1.m(); k += ts)
			for (size_t j{0}; j < m2.m(); j += ts) {
				Matrix A{std::min(m1.n() - i, ts), std::min(m1.m() - k, ts)}; MYMAC1(m1, {i, k}, A);
				Matrix B{std::min(m1.m() - k, ts), std::min(m2.m() - j, ts)}; MYMAC1(m2, {k, j}, B);
				Matrix C{std::min(m1.n() - i, ts), std::min(m2.m() - j, ts)};
				MYDGMMMAC_better_order(A, B, C); MYMAC2(C, m3, {i, j});
			}
	return m3;
}

Matrix multiply_mkl(const Matrix& m1, const Matrix& m2) {
	Matrix m3{m1.n(), m2.m()};

	cblas_dgemm(
			CblasRowMajor,	/* const CBLAS_LAYOUT Layout */
			CblasNoTrans,	/* const CBLAS_TRANSPOSE transa */
			CblasNoTrans,	/* const CBLAS_TRANSPOSE transb */
			m1.n(),			/* const MKL_INT m */
			m2.m(),			/* const MKL_INT n */
			m1.m(),			/* const MKL_INT k */
			1.0,			/* const double alpha */
			m1.data(),		/* const double *a */
			m1.m(),			/* const MKL_INT lda */
			m2.data(),		/* const double *b */
			m2.m(),			/* const MKL_INT ldb */
			0.0,			/* const double beta */
			m3.data(),		/* double * c */
			m3.m()			/* const MKL_INT ldc */
	);

	return m3;
}

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "pybind11 plugin"; // optional module docstring

	m.def("multiply_naive", &multiply_naive);
	m.def("multiply_naive_better_order", &multiply_naive_better_order);
	m.def("multiply_tile", &multiply_tile);
	m.def("multiply_tile_better_order", &multiply_tile_better_order);
	m.def("multiply_mkl", &multiply_mkl);
	m.def("bytes", &CustomAllocator<double>::bytes);
	m.def("allocated", &CustomAllocator<double>::allocated);
	m.def("deallocated", &CustomAllocator<double>::deallocated);
	py::class_<Matrix>(m, "Matrix")
		.def(py::init<size_t, size_t>())
		.def_property_readonly("nrow", &Matrix::n)
		.def_property_readonly("ncol", &Matrix::m)
		.def("__getitem__", &Matrix::operator[])
		.def("__setitem__", [](Matrix& mat, std::array<size_t, 2> idx, double d) { mat[idx] = d; })
		.def("__eq__", &Matrix::operator==)
		.def("__ne__", &Matrix::operator!=)
		.def("__str__", &Matrix::str);
}
