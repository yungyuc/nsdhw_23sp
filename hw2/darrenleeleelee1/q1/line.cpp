#include <iostream>
#include <vector>
class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    // Custom constructor.
    Line(size_t size) : m_size(size) { this->m_vector_x.resize(size), this->m_vector_y.resize(size); }
    ~Line() = default;
    size_t size() const { return m_size; }
    float const & x(size_t it) const { return m_vector_x.at(it); }
    float & x(size_t it) { return m_vector_x.at(it); };
    float const & y(size_t it) const { return m_vector_y.at(it); };
    float & y(size_t it) { return m_vector_y.at(it); };
private:
    // Member data.
    std::vector<float> m_vector_x;
    std::vector<float> m_vector_y;
    size_t m_size;
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}