#include <iostream>
#include <cassert>
#include <vector>

template<typename T>
class Line_template {
public:
    Line_template();
    Line_template(Line_template<T> const &other) {
        assert(other.px.size() == other.py.size());
        px = other.px; py = other.py;
    }
    Line_template(size_t size, T val = 0) : 
        px(size, val), py(size, val) {}  
    size_t size() {
        return px.size();
    }
    T const &x(size_t it) const {
        return px[it];
    }
    T const &y(size_t it) const {
        return py[it];
    }
    T &x(size_t it) {
        return px[it];
    }
    T &y(size_t it) {
        return py[it];
    }
    Line_template &operator=(Line_template<T> const &rhs) {
        px = rhs.px;
        py = rhs.py;
        return *this;
    }
    ~Line_template() {}
private:
    std::vector<T> px, py;
};

using Line = Line_template<float>;


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