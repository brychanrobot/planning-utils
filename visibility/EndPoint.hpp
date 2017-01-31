#pragma once

#include <cstdlib>
#include "../geom/Coord.hpp"

class Segment;  // forward declaration. This is how we includ a class which has an instance of this class

class EndPoint {
   protected:
   public:
	double x;
	double y;
	bool beginsSegment = false;
	double angle = 0.0;
	std::shared_ptr<Segment> segment = nullptr;

	EndPoint() {}
	EndPoint(Coord point) {
		this->x = point.x;
		this->y = point.y;
	}
};