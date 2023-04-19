#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <pybind11/pybind11.h>
#include <mkl.h>
#include <ctime> 

using namespace std ;

template<class T>
class MyAllocator {
  private:
    static size_t allocate_byte ;
    static size_t deallocate_byte ;
    static size_t _bytes ;

  public:    
    using value_type =  T ;

    T * allocate(size_t n) {
      if (n > numeric_limits<std::size_t>::max() / sizeof(T))
        throw bad_alloc();
      else {
        T * ptr = (T*)malloc(sizeof(T)*n) ;
        if ( ptr ) {
          allocate_byte = allocate_byte + sizeof(T)*n ;
          _bytes = _bytes + sizeof(T)*n  ;
          return ptr ;
        } // if
        else 
          throw bad_alloc(); 
      } // else 
    } // allocate()

    void deallocate(T* ptr, size_t n) noexcept {
        deallocate_byte = deallocate_byte + sizeof(T)*n ;
        _bytes = _bytes - sizeof(T)*n  ;
        std::free(ptr);
    } // deallocate()

    static size_t allocated() {
      return allocate_byte ;
    }   
    static size_t deallocated() {
      return deallocate_byte ;
    }   
    static size_t bytes() {
      return _bytes;
    }   
}; 

class Matrix {

private:

    size_t m_nrow;
    size_t m_ncol;

public:
    vector<double,MyAllocator<double>> m_buffer ;

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(vector<double,MyAllocator<double>>(m_nrow*ncol, 0)){} // origin constructor

    // Matrix(Matrix const & temp)
    //   : m_nrow(temp.nrow()), m_ncol(temp.ncol()), m_buffer(vector<double,MyAllocator<double>>(m_nrow*m_ncol, 0))
    // {
    //   copy(temp.m_buffer.begin(), temp.m_buffer.end(), m_buffer.begin());
    // } // copy constructor


    // ~Matrix()
    // {
    //     m_buffer.clear();
    // }

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
    double * data_ptr() const { return (double*)m_buffer.data() ; }
}; // Matrix()

template<class T>
size_t MyAllocator<T>::allocate_byte = 0 ;
template<class T>
size_t MyAllocator<T>::deallocate_byte = 0 ;
template<class T>
size_t MyAllocator<T>::_bytes = 0 ;

bool matrix_equal( Matrix const & mat1, Matrix const & mat2 ) {
  if ( (mat1.nrow() == mat2.nrow()) && (mat1.ncol() == mat2.ncol()) ) {
    for ( size_t i = 0 ; i < mat1.nrow()*mat1.ncol() ; i++ ) {
      if ( mat1.m_buffer[i] != mat2.m_buffer[i] ) { 
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
    Matrix nil(1.0,1.0) ;
    return nil ;
  } // if  
  Matrix ans_mat(mat1.nrow(), mat2.ncol()) ;
  ans_mat.set_buffer(0.0) ;
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
    Matrix nil(1.0,1.0) ;
    return nil ;
  } // if 
  Matrix result(mat1.nrow(), mat2.ncol()) ;
  result.set_buffer(0.0) ;
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


Matrix multiply_mkl( const Matrix & mat1, const Matrix & mat2 ) {
  if ( mat1.ncol() != mat2.nrow() ){
    cerr << "There is an error!" << endl ;
    Matrix nil(1.0,1.0) ;
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
              mat1.data_ptr() /* const double *a */
              ,
              mat1.ncol() /* const MKL_INT lda */
              ,
              mat2.data_ptr() /* const double *b */
              ,
              mat2.ncol() /* const MKL_INT ldb */
              ,
              0.0 /* const double beta */
              ,
              result.data_ptr() /* double * c */
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
    m.def("allocated", &MyAllocator<double>::allocated);
    m.def("deallocated", &MyAllocator<double>::deallocated);
    m.def("bytes", &MyAllocator<double>::bytes);
    pybind11::class_<Matrix>(m, "Matrix")
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
                                { return mat.ncol(); });
}

// int main(int argc, char ** argv) {
//     size_t width = 8;

//     Matrix matrix(width, width) ;
//     Matrix matrix2(width, width) ;

//     populate(matrix, 1);
//     populate(matrix2, 2);
//     clock_t begin_naive, end_naive ;
//     clock_t begin_tile, end_tile ;

//     begin_naive = clock() ;
//     Matrix answer(multiply_naive(matrix, matrix2)) ;
//     end_naive = clock() ;
//     double naive_time = double( end_naive - begin_naive ) / CLOCKS_PER_SEC ;
//     cout << "naive time:" << naive_time << endl ;
    
//     begin_tile = clock() ;
//     Matrix answer2(multiply_tile(matrix, matrix2, 32)) ;
//     end_tile = clock() ;
//     double tile_time = double( end_tile - begin_tile ) / CLOCKS_PER_SEC ;
//     cout << "tile time:" << tile_time << endl ;
//     cout << "time_rate:" << tile_time / naive_time << endl ;
//     // Matrix answer3(mulltiply_mkl(matrix, matrix2)) ;

//     std::cout << "naive:";
//     for (size_t i=0; i<answer.nrow(); ++i) // the i-th row
//     {
//         std::cout << std::endl << " ";
//         for (size_t j=0; j<answer.ncol(); ++j) // the j-th column
//         {
//             std::cout << " " << std::setfill('0') << std::setw(2)
//                       << answer(i, j);
//         }
//     }
//     std::cout << std::endl;

//     std::cout << "tile:";
//     for (size_t i=0; i<answer2.nrow(); ++i) // the i-th row
//     {
//         std::cout << std::endl << " ";
//         for (size_t j=0; j<answer2.ncol(); ++j) // the j-th column
//         {
//             std::cout << " " << std::setfill('0') << std::setw(2)
//                       << answer2(i, j);
//         }
//     }
//     std::cout << std::endl;
//     MyAllocator<double> alct ;
//     cout << "Allocated:" << alct.allocated() << endl 
//          << "Deallocated:" << alct.deallocated() << endl
//          << "bytes:" << alct.bytes() << endl ;

//     // std::cout << "mkl:";
//     // for (size_t i=0; i<answer3.nrow(); ++i) // the i-th row
//     // {
//     //     std::cout << std::endl << " ";
//     //     for (size_t j=0; j<answer3.ncol(); ++j) // the j-th column
//     //     {
//     //         std::cout << " " << std::setfill('0') << std::setw(2)
//     //                   << answer3(i, j);
//     //     }
//     // }
//     // std::cout << std::endl;
    
//     return 0;
// }