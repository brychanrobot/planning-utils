#pragma once

#include <cstdlib>
#include "../geom/Coord.hpp"

class EndPoint;  // forward declaration. This is how we includ a class which has an instance of this class

class Segment {
   protected:
   public:
	std::shared_ptr<EndPoint> p1;
	std::shared_ptr<EndPoint> p2;
	double d = 0.0;
	// Don't use this by itself'
	Segment(Coord p1, Coord p2) {
		this->p1 = std::make_shared<EndPoint>(p1);
		this->p2 = std::make_shared<EndPoint>(p2);
	}
};

inline std::shared_ptr<Segment> createSegment(Coord p1, Coord p2) {
	auto segment = std::make_shared<Segment>(p1, p2);
	segment->p1->segment = segment;
	segment->p2->segment = segment;
	return segment;
}