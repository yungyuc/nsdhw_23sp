#pragma once

#include <cstddef>
#include <vector>

class Line
{
public:
    typedef std::pair<float, float> Point;
    // Basic constructors.
    Line() = default; // default constructor.
    Line(Line const & ) = default; // copy constructor.
    Line(Line       &&) = default; // move constructor.
    Line & operator=(Line const & ) = default; // copy assignment operator.
    Line & operator=(Line       &&) = default; // move assignment operator.
    // Custom constructor.
    Line(size_t size) : points(size) {}
    // Desctructor.
    ~Line() = default;
    // Accessors.
    size_t size() const { return points.size(); }
    float   x(int i) const { return points[i].first; }
    float & x(int i)       { return points[i].first; }
    float   y(int i) const { return points[i].second; }
    float & y(int i)       { return points[i].second; }
private:
    // Member data.
    std::vector<Point> points;
}; /* end class Line */
