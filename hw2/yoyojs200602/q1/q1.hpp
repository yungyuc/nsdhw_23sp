#include <vector>
#include <iostream>

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
    float const & x(size_t it) const;
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
    std::vector<float> X;
    std::vector<float> Y;
}; /* end class Line */