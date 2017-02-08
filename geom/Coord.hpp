#pragma once

/*
struct Coord {
  double x;
  double y;
  Coord(){}
  Coord(int x, int y) : x(x), y(y) {}
};
*/
#include <cmath>
#include <functional>
#include <vector>

#include <boost/geometry/geometries/geometries.hpp>
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point;

class Coord {
   private:
   public:
	double x;
	double y;

	Coord() {}
	Coord(double x, double y) {
		this->x = x;
		this->y = y;
	}

	// double x() { return this->c[0]; }

	// double y() { return this->c[1]; }

	void change(double x, double y) {
		this->x = x;
		this->y = y;
	}

	point getBoostPoint() { return point(this->x, this->y); }

	// bool operator==(const Coord& other) { return this->x == other.x && this->y == other.y; }
};

inline bool operator==(const Coord& lhs, const Coord& rhs) { return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) < 0.001; }
inline bool operator!=(const Coord& lhs, const Coord& rhs) { return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) > 0.001; }

namespace std {

template <>
struct hash<Coord> {
	std::size_t operator()(const Coord& c) const {
		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return (std::hash<double>()(c.x) ^ (std::hash<double>()(c.y) << 1));
	}
};
}