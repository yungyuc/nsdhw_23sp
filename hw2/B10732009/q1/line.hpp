#pragma once

#include <vector>

class Line
{
public:
    Line();                        // default contructor
    Line(Line const &);            // copy constructor
    Line(Line &&);                 // move constructor
    Line &operator=(Line const &); // copy assignment operator
    Line &operator=(Line &&);      // move assignment operator
    ~Line();                       // destructor

    Line(std::size_t size);
    std::size_t size() const;
    double const &x(std::size_t it) const;
    double &x(std::size_t it);
    double const &y(std::size_t it) const;
    double &y(std::size_t it);

private:
    // Member data.
    std::size_t dataSize;
    std::vector<double> dataX;
    std::vector<double> dataY;
}; /* end class Line */
