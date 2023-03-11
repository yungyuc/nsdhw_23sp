#include "Line.h"

Line::Line(size_t size)
{
    m_size = size;
    m_x.resize(size);
    m_y.resize(size);
}

int &Line::x(size_t index)
{
    return m_x[index];
}

int &Line::y(size_t index)
{
    return m_y[index];
}

size_t Line::size()
{
    return m_size;
}
