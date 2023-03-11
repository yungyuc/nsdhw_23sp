#include <vector>
#include <iostream>
#include "q1.hpp"

Line::Line(){};

Line::Line(Line const &line): X(line.X), Y(line.Y) {};

Line::Line(Line &&line): X(std::move(line.X)), Y(std::move(line.Y)) {};

Line & Line::operator=(Line const &line)
{
    if (this != &line) 
    {
        X = line.X;
        Y = line.Y;
    }
    return *this;
};

Line & Line::operator=(Line &&line)
{
    X = std::move(line.X);
    Y = std::move(line.Y);
    return *this;
};

Line::Line(size_t size): X(size), Y(size) {};

Line::~Line() {};

size_t Line::size() const {return X.size();};

float const & Line::x(size_t it) const {return X[it];};

float & Line::x(size_t it) {return X[it];};

float const & Line::y(size_t it) const {return Y[it];};

float & Line::y(size_t it) {return Y[it];};


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