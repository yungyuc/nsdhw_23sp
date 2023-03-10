#ifndef LINE_HPP
#define LINE_HPP
#include <iostream>
#include<vector>
using namespace std;
class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    float const & x(size_t) const;
    float & x(size_t);
    float const & y(size_t) const;
    float & y(size_t);
private:
    // Member data.
    vector<float> m_x, m_y;
}; /* end class Line */
#endif