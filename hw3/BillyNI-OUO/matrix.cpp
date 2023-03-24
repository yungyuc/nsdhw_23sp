#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>
using namespace std;
namespace py=pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        m_buffer = new double[nrow*ncol];
    }

    ~Matrix()
    {
        delete[] m_buffer;
    }

    void setter(size_t row, size_t col, double val)
    {
    	
        m_buffer[row*m_ncol + col] = val;
    }
    double getter(size_t row, size_t col)
    {
    	if(row > m_nrow || col > m_ncol){
    		throw std::out_of_range("out of matrix index");
    	}
        return m_buffer[row*m_ncol + col];
    }
    double   operator() (size_t row, size_t col) const
    {
    	if(row > m_nrow || col > m_ncol){
    		throw std::out_of_range("out of matrix index");
    	}
        return m_buffer[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col)
    {
    	if(row > m_nrow || col > m_ncol){
    		throw std::out_of_range("out of matrix index");
    	}
        return m_buffer[row*m_ncol + col];
    }
    bool operator ==(Matrix r)
	{
		if(this==&r)
			return true;
		else if(this->nrow() != r.nrow() || this->ncol() != r.ncol()){
			return false;
		}else{
			for(size_t i = 0; i < this->nrow(); i ++){
				for(size_t j = 0; j < this->ncol(); j ++){
					if(this->getter(i,j) != r(i,j)){
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double* data()const{return m_buffer;}

private:

    size_t m_nrow;
    size_t m_ncol;
    double* m_buffer;

};


Matrix multiply_naive(Matrix a, Matrix b){
	 if (a.ncol() != b.nrow())
    {
        throw std::out_of_range("matrix column differs from row size");
    }

    Matrix ret = Matrix(a.nrow(), b.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<a.ncol(); ++j)
            {
                v += a(i,j) * b(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

Matrix multiple_tile(Matrix a, Matrix b){
	if (a.ncol() != b.nrow())
    {
        throw std::out_of_range("matrix column differs from row size");
    }

    Matrix ret = Matrix(a.nrow(), b.ncol());
    int T = 16;
    int M = a.ncol();
    int N = a.nrow();
    int K = b.nrow();
    for (int m = 0; m < M/T; m += T) {
        for (int n = 0; n < N/T; n += T) {
            for (int k = 0; k < K/T; k += T) {

                const int minMt = std::min(m + T, M);
                const int minNt = std::min(n + T, N);
                const int minKt = std::min(k + T, K);

                for (int mt = m; mt < minMt; mt++) {
                    for (int nt = n; nt < minNt; nt++) {
                        for (int kt = k; kt < minKt; kt++) {
                            ret(mt * M, nt) += a(mt * M, kt) * b(kt * K , nt);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix const &a, Matrix const &b)
{
    if (a.ncol() != b.nrow())
    {
        throw std::out_of_range("matrix column differs from row size");
    }


    Matrix ret = Matrix(a.nrow(), b.ncol());

    cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transa */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transb */
                ,
                a.nrow() /* const MKL_INT m */
                ,
                b.ncol() /* const MKL_INT n */
                ,
                a.ncol() /* const MKL_INT k */
                ,
                1.0 /* const double alpha */
                ,
                a.data() /* const double *a */
                ,
                a.ncol() /* const MKL_INT lda */
                ,
                b.data() /* const double *b */
                ,
                b.ncol() /* const MKL_INT ldb */
                ,
                0.0 /* const double beta */
                ,
                ret.data() /* double * c */
                ,
                ret.ncol() /* const MKL_INT ldc */
    );
    return ret;
}



PYBIND11_MODULE(_matrix, m) {
	// py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
	// .def(py::init<int, int>())
   	// .def_buffer([](Matrix &m) -> py::buffer_info {
    //     return py::buffer_info(
    //         m.data(),                               /* Pointer to buffer */
    //         sizeof(float),                          /* Size of one scalar */
    //         py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
    //         2,                                      /* Number of dimensions */
    //         { m.nrow(), m.ncol() },                 /* Buffer dimensions */
    //         { sizeof(float) * m.ncol(),             /* Strides (in bytes) for each index */
    //           sizeof(float) }
    //     );
    // });
    py::class_<Matrix>(m, "Matrix")
    .def(py::init<int, int>())
    .def("__getitem__", [](Matrix &self, pybind11::args args)
            { 	 
             	//std::cout << (*args[0]) << std::endl;
            	py::tuple t = args[0];
            	// for (size_t it=0; it<t.size(); ++it) {
        		// 	py::print(py::str("{}").format(t[it].cast<int>()));
      			// }
            	return self(t[0].cast<int>(),t[1].cast<int>()); 
         	})
    .def("__setitem__",[](Matrix &self, pybind11::args args)
            { 	 
            	// std::cout << (*args[0]) << " "<< *args[1] << std::endl;
            	py::tuple t = args[0];	
            	self(t[0].cast<int>(),t[1].cast<int>()) = args[1].cast<int>(); 
         	})
    .def_property_readonly("nrow", &Matrix::nrow)
    .def_property_readonly("ncol", &Matrix::ncol)
    .def("__eq__", &Matrix::operator ==);

    m.def("multiply_naive", &multiply_naive, "basic Matrix-Matrix Multiplication");
    m.def("multiple_tile", &multiple_tile, "tile Matrix-Matrix Multiplication");
    m.def("multiply_mkl", &multiply_mkl, "mkl Matrix-Matrix Multiplication");
}


// def __setitem__(self, key, value):
//         setattr(self, key, value)
//[](Vec &self, unsigned index, float val)
         //    { *self[index] = val; })

//     def __getitem__(self, key):
//         return getattr(self, key)
//static_cast<void (Pet::*)(int)>(&Pet::set)