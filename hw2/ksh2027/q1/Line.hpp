#pragma once
#include <cstdint>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>

class Line
{
public:
    // Basic constructors.
    Line();
    // Custom constructor.
    Line(size_t size);
    // Desctructor.
    ~Line();
    // Accessors.
    size_t size() const;
    float & x(size_t it);
    float & y(size_t it);
private:
    // Private helper.
    void check_range(size_t it) const;
    // Member data.
    std::vector<std::vector<float>> coor;
}; /* end class Line */