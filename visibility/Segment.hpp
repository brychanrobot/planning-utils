#include <cstdlib>
#include "EndPoint.hpp"

inline std::shared_ptr<Segment> createSegment(Coord p1, Coord p2) {
	auto segment = std::make_shared<Segment>(p1, p2);
	segment->p1.segment = segment;
	segment->p2.segment = segment;
	return segment;
}

struct Segment {
   protected:
   public:
	EndPoint p1;
	EndPoint p2;
	double d = 0.0;
	// Don't use this by itself'
	Segment(Coord p1, Coord p2) {
		this->p1 = EndPoint(p1);
		this->p2 = EndPoint(p2);
	}
}