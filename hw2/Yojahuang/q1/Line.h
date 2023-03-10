#ifndef LINE_H
#define LINE_H

#include <cstddef>
#include <vector>

class Line
{
public:
    Line();
    Line(Line const & );
    Line(size_t size);
    ~Line();
    size_t size() const;
    float & x(size_t it);
    float const & x(size_t it) const;
    float & y(size_t it);
    float const & y(size_t it) const;
private:
    // Member data.
    std::vector<float> m_x, m_y;
    size_t m_size;
}; /* end class Line */


#endif