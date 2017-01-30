#include <cstdlib>
#include "../geom/Coord.hpp"

class EndPoint {
   protected:
   public:
	Coord point;
	bool beginsSegment = false;
	double angle = 0.0;
	std::shared_ptr<Segment> segment = nullptr;

	EndPoint(Coord point) { this->point = point; }
}