#include <iostream>
#include <vector>
#include <utility> // std::size_t

class Line {
	// Member data.
	std::vector<std::pair<float, float>> pt{};
public:
	Line()=default;
	Line(const Line& other)=default;
	Line(Line&& other)=default;
	Line& operator=(const Line& other)=default;
	Line& operator=(Line&& other)=default;
	Line(std::size_t size) : pt(size) {}
	~Line()=default;
	std::size_t size() const { return pt.size(); }
	const float& x(std::size_t it) const { return pt[it].first; }
	float& x(std::size_t it)  { return pt[it].first; }
	const float& y(std::size_t it) const { return pt[it].second; }
	float& y(std::size_t it) { return pt[it].second; }
}; /* end class Line */

int main(int, char **) {
	Line line{3};
	line.x(0) = 0, line.y(0) = 1;
	line.x(1) = 1, line.y(1) = 3;
	line.x(2) = 2, line.y(2) = 5;

	Line line2{line};
	line2.x(0) = 9;

	std::cout << "line: number of points = " << line.size() << std::endl;
	for (std::size_t it{0}; it < line.size(); ++it) {
		std::cout << "point " << it << ":"
				  << " x = " << line.x(it)
				  << " y = " << line.y(it) << std::endl;
	}

	std::cout << "line2: number of points = " << line2.size() << std::endl;
	for (std::size_t it{0}; it < line2.size(); ++it) {
		std::cout << "point " << it << ":"
				  << " x = " << line2.x(it)
				  << " y = " << line2.y(it) << std::endl;
	}

	return 0;
}
