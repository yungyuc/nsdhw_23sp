#include <iostream>
#include <iomanip>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <mkl.h>
#include <ctime> 

using namespace std ;
class Matrix {

private:

    size_t m_nrow;
    size_t m_ncol;

public:
    double * m_buffer;

    double * my_buffer() { return m_buffer ; }

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }
    Matrix(Matrix const & temp)
      : m_nrow(temp.nrow()), m_ncol(temp.ncol())
    {
      m_buffer = new double[m_nrow*m_ncol] ;
      for ( size_t i = 0 ; i < m_nrow*m_ncol ; i++ )
        m_buffer[i] = temp.m_buffer[i] ;
    } // copy constructor

    // Matrix & operator=( Matrix const & temp ) {
    //   if ( &temp == this )
    //     return *this ;
    //   m_nrow = temp.nrow() ;
    //   m_ncol = temp.ncol() ;
    //   m_buffer = new double[m_nrow*m_ncol] ;
    //   for ( size_t i = 0 ; i < m_nrow*m_ncol ; i++ )
    //     m_buffer[i] = temp.m_buffer[i] ;
    // } // copy assignment constructor

    ~Matrix()
    {
        delete[] m_buffer;
    }

    bool operator==(const Matrix &other) const {
        if (nrow() != other.nrow() || ncol() != other.ncol())
            return false;
        for (size_t i = 0; i < m_nrow; i++) {
            for (size_t j = 0; j < m_ncol; j++) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[row*m_ncol + col];
    }

    void set_buffer( double num ) {
      for ( size_t i = 0 ; i < m_nrow*m_ncol ; i++ )
        m_buffer[i] = num ;
    } // set_buffer()

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
}; // Matrix()

bool matrix_equal( Matrix const & mat1, Matrix const & mat2 ) {
  if ( (mat1.nrow() == mat2.nrow()) && (mat1.ncol() == mat2.ncol()) ) {
    for ( size_t i = 0 ; i < mat1.nrow()*mat1.ncol() ; i++ ) {
      if ( mat1.m_buffer[i] != mat2.m_buffer[i] ) {
        //cout << "index:" << i << endl ; 
        return false ;
      } // if   
    } // 
  } // if  
  else 
    return false ;
  
  return true ;
} // matrix_equal()

void populate(Matrix & matrix, double num )
{
    for (size_t i=0; i<matrix.nrow(); ++i) // the i-th row
    {
        for (size_t j=0; j<matrix.ncol(); ++j) // the j-th column
        {
            matrix(i, j) = num ;
            //num = num + 1 ;
        }
    }
}

Matrix multiply_naive( Matrix & mat1, Matrix & mat2 ) {
  if ( mat1.ncol() != mat2.nrow() ){
    cerr << "There is an error!" << endl ;
    Matrix nil(1,1) ;
    return nil ;
  } // if  
  Matrix ans_mat(mat1.nrow(), mat2.ncol()) ;
  ans_mat.set_buffer(0) ;
  for (size_t i=0; i<ans_mat.nrow(); ++i) {
    for (size_t j=0; j<ans_mat.ncol(); ++j) {
      for ( size_t k = 0 ; k < mat1.ncol(); k++ ) {
        ans_mat(i, j) = ans_mat(i, j) + mat1(i, k) * mat2(k, j) ;
      } // for   
    } // for 
  } // for 
  return ans_mat ;
} // mutiply_naive()

Matrix multiply_tile( Matrix & mat1, Matrix & mat2, size_t stride ) {
  if ( mat1.ncol() != mat2.nrow() ){
    cerr << "There is an error!" << endl ;
    Matrix nil(1,1) ;
    return nil ;
  } // if 
  Matrix result(mat1.nrow(), mat2.ncol()) ;
  result.set_buffer(0) ;
  for (size_t m = 0; m < mat2.ncol(); m += stride) {
    for (size_t n = 0; n < mat1.nrow(); n += stride) {
      for (size_t k = 0; k < mat1.ncol(); k += stride) {
        for (size_t mt = m; mt < m + stride && mt < mat2.ncol(); mt++) {
          for (size_t nt = n; nt < n + stride && nt < mat1.nrow(); nt++) {
            for (size_t kt = k; kt < k + stride && kt < mat1.ncol(); kt++) {
              result(mt, nt) = result(mt, nt) + mat1(mt,kt)*mat2(kt,nt) ;
            } // for 
          } // for 
        } // for 
      } // for 
    } // for 
  } // for

  return result ;
} // mutiply_tile()


Matrix multiply_mkl( Matrix const & mat1, Matrix const & mat2 ) {
  if ( mat1.ncol() != mat2.nrow() ){
    cerr << "There is an error!" << endl ;
    Matrix nil(1,1) ;
    return nil ;
  } // if 
  Matrix result(mat1.nrow(), mat2.ncol()) ;  
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
              result.m_buffer /* double * c */
              ,
              result.ncol() /* const MKL_INT ldc */
    );
    return result ;

} // mutiply_mkl()

PYBIND11_MODULE(_matrix, m){
    m.doc() = "pybind11 matrix";
    m.def("populate", &populate);
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("matrix_equal", &matrix_equal);
    pybind11::class_<Matrix>(m, "Matrix", pybind11::buffer_protocol())
        .def( pybind11::init<size_t, size_t>())
        .def( pybind11::init<Matrix>())
        .def( "set_buffer", &Matrix::set_buffer )
        .def("__setitem__",
             [](Matrix &self, std::pair<size_t, size_t> idx, double val) {
                 self(idx.first, idx.second) = val;
             })
        .def("__getitem__",
             [](const Matrix &self, std::pair<size_t, size_t> idx) {
                 return self(idx.first, idx.second);
             })
        .def("__eq__", [](const Matrix &a, const Matrix &b) { return a == b; })
        .def_property_readonly("nrow", [](const Matrix &mat)
                                { return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat)
                                { return mat.ncol(); })
        .def_property_readonly("array", [](Matrix &m)
                                {
                                  size_t _size = 8 ;
                                  return pybind11::array_t<double>(
                                    {m.nrow(), m.ncol()},
                                    {_size*m.ncol(), _size},
                                    m.my_buffer(),
                                    pybind11::cast(m));
                                });
}