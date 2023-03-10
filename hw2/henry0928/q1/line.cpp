#include <iostream>
#include <vector>
#include <algorithm>

using namespace std ;
class Line
{
public:
    Line(): point_count(0), x_list(0), y_list(0) {}
    Line(size_t size): point_count(size), x_list(size), y_list(size) {} 
    ~Line(){
      point_count = 0 ;
      x_list.clear() ;
      y_list.clear() ;
    } // ~Line() 
    Line(Line const & instance): point_count(instance.point_count) 
                                , x_list(instance.x_list), y_list(instance.y_list) {} // copy constructor()
    Line(Line && instance ): point_count(instance.point_count) 
                            , x_list(move(instance.x_list)), y_list(move(instance.y_list)) {} // move constructor()
    Line & operator=(Line const & instance) {
      if ( this == &instance )
        return *this ;
      point_count = instance.point_count ;
      x_list.clear() ;
      y_list.clear() ;
      copy_list(instance) ;
      return *this ;
    } // copy assignment constructor() 
    Line & operator=(Line && instance) {
      if ( this == &instance )
        return *this ;
      point_count = instance.point_count ;
      x_list = move(instance.x_list) ;
      y_list = move(instance.y_list) ;
      return *this ;
    } // move assignment constructor()
    size_t size() const { return point_count ; } 
    float const & x(size_t it) const { return x_list[it] ; }
    float & x(size_t it) { return x_list[it] ; }
    float const & y(size_t it) const { return y_list[it] ; }
    float & y(size_t it) { return y_list[it] ; }
private:
    // Member data.
    vector<float> x_list ;
    vector<float> y_list ;
    size_t point_count ;

    void copy_list( Line const & item ) {
      size_t i = 0 ;
      while ( i < item.point_count ) {
        x_list.push_back(item.x_list[i]) ;
        y_list.push_back(item.y_list[i]) ;
        i++ ;
      } // while 
    } // copy_x_list()
  
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    cout << "line: number of points = " << line.size() << endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
             << " x = " << line.x(it)
             << " y = " << line.y(it) << endl;
    }
    //cout << "Pass origin constructor\n\n" ;

    Line line2(line);
    line2.x(0) = 9;
    cout << "line2: number of points = " << line.size() << endl; // copy constructor()
    for (size_t it=0; it<line.size(); ++it)
    {
      cout << "point " << it << ":"
           << " x = " << line2.x(it)
           << " y = " << line2.y(it) << endl;
    }
    //cout << "Pass copy constructor\n\n" ;
    
    // Line line3(move(line2));
    // cout << "line3: number of points = " << line3.size() << endl; // move constructor()

    // for (size_t it=0; it<line.size(); ++it)
    // {
    //   cout << "point " << it << ":"
    //        << " x = " << line3.x(it)
    //        << " y = " << line3.y(it) << endl;
    // } 
    // cout << "Pass move constructor\n\n" ;

    // Line line4 = line3 ;
    // line4.x(0) = 99;
    // cout << "line4: number of points = " << line4.size() << endl; // copy assignment constructor()

    // for (size_t it=0; it<line4.size(); ++it)
    // {
    //   cout << "point " << it << ":"
    //        << " x = " << line4.x(it)
    //        << " y = " << line4.y(it) << endl;
    // }
    // cout << "Pass copy assignment constructor\n\n" ;

    // Line line5 = move(line4) ;
    // cout << "line5: number of points = " << line5.size() << endl; // move assignment constructor()

    // for (size_t it=0; it<line5.size(); ++it)
    // {
    //   cout << "point " << it << ":"
    //        << " x = " << line5.x(it)
    //        << " y = " << line5.y(it) << endl;
    // } 
    // cout << "Pass move assignment constructor\n\n" ;
    return 0;
}