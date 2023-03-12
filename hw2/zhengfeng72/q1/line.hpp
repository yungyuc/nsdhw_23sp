#include <iostream>
#include <vector>

class Line{
public:
    Line() = default;
    Line(Line const & ); // copy constructor
    Line(Line       &&); // move constrtuctor
    Line & operator=(Line const & ); // copy assignment operator
    Line & operator=(Line       &&); // move assignment operator
    Line(size_t size);
    ~Line() = default;
    size_t size() const;
    float  x(size_t it) const; // getter
    float & x(size_t it); // setter
    float  y(size_t it) const; // getter
    float & y(size_t it); // setter
private:
    // Member data.
    std::vector<float> m_coord_x;
    std::vector<float> m_coord_y;
};