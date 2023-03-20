#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <cstddef>
#include <vector>

class Line
{
private:
    size_t m_size;
    std::vector<int> m_x, m_y;

public:
    Line(size_t size);
    int &x(size_t index);
    int &y(size_t index);
    size_t size();
};

#endif // LINE_H_INCLUDED