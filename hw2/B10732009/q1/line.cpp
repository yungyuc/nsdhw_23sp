#include "line.hpp"

// default contructor
Line::Line() : dataSize(0), dataX({}), dataY({}){};

// copy constructor
Line::Line(const Line &line) : dataSize(line.dataSize), dataX(line.dataX), dataY(line.dataY) {}

// move constructor
Line::Line(Line &&line) : dataSize(line.dataSize), dataX(std::move(line.dataX)), dataY(std::move(line.dataY)) {}

// copy assignment operator
Line &Line::operator=(Line const &line)
{
    // self-assignment check
    if (this != &line)
    {
        dataSize = line.dataSize;
        dataX = line.dataX;
        dataY = line.dataY;
    }
    return *this;
}

// move assignment operator
Line &Line::operator=(Line &&line)
{
    // self-assignment check
    if (this != &line)
    {
        dataSize = line.dataSize;
        dataX = std::move(line.dataX);
        dataY = std::move(line.dataY);
    }
    return *this;
}

// custom constructor
Line::Line(std::size_t _dataSize) : dataSize(_dataSize), dataX(std::vector<double>(_dataSize)), dataY(std::vector<double>(_dataSize)) {}

// destructor
Line::~Line() {}

// accessors
std::size_t Line::size() const { return dataSize; }
double const &Line::x(std::size_t it) const { return dataX.at(it); }
double &Line::x(std::size_t it) { return dataX.at(it); }
double const &Line::y(std::size_t it) const { return dataY.at(it); }
double &Line::y(std::size_t it) { return dataY.at(it); }