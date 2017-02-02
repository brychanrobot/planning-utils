#pragma once

#include "../geom/Coord.hpp"
#include "./Segment.hpp"

bool leftOf(std::shared_ptr<Segment> segment, Coord point) {
	auto cross = (segment->p2->x - segment->p1->x) * (point.y - segment->p1->y) - (segment->p2->y - segment->p1->y) * (point.x - segment->p1->x);
	return cross < 0;
};

Coord interpolate(std::shared_ptr<EndPoint> pointA, std::shared_ptr<EndPoint> pointB, double f) {
	return Coord(pointA->x * (1 - f) + pointB->x * f, pointA->y * (1 - f) + pointB->y * f);
};

bool segmentInFrontOf(std::shared_ptr<Segment> segmentA, std::shared_ptr<Segment> segmentB, Coord relativePoint) {
	auto A1 = leftOf(segmentA, interpolate(segmentB->p1, segmentB->p2, 0.01));
	auto A2 = leftOf(segmentA, interpolate(segmentB->p2, segmentB->p1, 0.01));
	auto A3 = leftOf(segmentA, relativePoint);
	auto B1 = leftOf(segmentB, interpolate(segmentA->p1, segmentA->p2, 0.01));
	auto B2 = leftOf(segmentB, interpolate(segmentA->p2, segmentA->p1, 0.01));
	auto B3 = leftOf(segmentB, relativePoint);

	if (B1 == B2 && B2 != B3) {
		return true;
	}
	if (A1 == A2 && A2 == A3) {
		return true;
	}
	/*if (A1 == A2 && A2 != A3) {
	    return false;
	}
	if (B1 == B2 && B2 == B3) {
	    return false;
	}*/

	return false;
}