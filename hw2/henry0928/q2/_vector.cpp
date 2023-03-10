#include <iostream> 
#include <cmath>
#include <pybind11/pybind11.h>

using namespace std ;
namespace py = pybind11 ;

class vec {
    private:
      double x1_vector ;
      double y1_vector ;
      double x2_vector ;
      double y2_vector ;
    public:   
      //vec(): x1_vector(0), y1_vector(0), x2_vector(0), y2_vector(0){}
      vec(double x1, double y1, double x2, double y2)
                : x1_vector(x1), y1_vector(y1), x2_vector(x2), y2_vector(y2){}

      double angle() {
        if ( length_(x1_vector, y1_vector) != 0 && length_(x2_vector, y2_vector) != 0 ) {
          double _cos = inner_product() / ( length_(x1_vector, y1_vector) * length_(x2_vector, y2_vector) ) ;
          if ( _cos <= 1 && _cos >= -1 ) 
            return acos(_cos) ;
          else
            cout << "Cos_value is invalid" << endl ;
        } // if
        else 
          cout << "Will divided zero" << endl ;
        return -2 ;
      } //angle()

      double inner_product() {
        return x1_vector*x2_vector + y1_vector*y2_vector ; 
      } // inner_product()

      double length_( double x, double y ) {
        return sqrt(x*x + y*y) ; 
      } // length_

} ; // vec()

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 example" ;
  py::class_<vec>(m, "vec")
      .def(py::init<double,double,double,double>())
      .def("angle", &vec::angle)
      .def("inner_product", &vec::inner_product)
      .def("length_", &vec::length_) ;
} 