#pragma once

#include <algorithm>
#include <cmath>
#include <list>
#include <vector>

#include "../geom/Coord.hpp"
#include "./loadMap.hpp"
#include "./segmentInFrontOf.hpp"

inline bool endpointCompare(const std::shared_ptr<EndPoint>& pointA, const std::shared_ptr<EndPoint>& pointB) {
	if (pointA->angle > pointB->angle) return false;
	if (pointA->angle < pointB->angle) return true;
	if (!pointA->beginsSegment && pointB->beginsSegment) return false;
	if (pointA->beginsSegment && !pointB->beginsSegment) return true;
	return false;
}

inline Coord lineIntersection(Coord p1, Coord p2, Coord p3, Coord p4) {
	auto s = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

	return Coord(p1.x + s * (p2.x - p1.x), p1.y + s * (p2.y - p1.y));
}

inline std::vector<Coord> getTrianglePoints(Coord origin, double angle1, double angle2, std::shared_ptr<Segment> segment) {
	auto p1 = origin;
	auto p2 = Coord(origin.x + cos(angle1), origin.y + sin(angle1));
	auto p3 = Coord(0, 0);
	auto p4 = Coord(0, 0);

	if (segment != nullptr) {
		p3.x = segment->p1->x;
		p3.y = segment->p1->y;
		p4.x = segment->p2->x;
		p4.y = segment->p2->y;
	} else {
		p3.x = origin.x + cos(angle1) * 500;
		p3.y = origin.y + sin(angle1) * 500;
		p4.x = origin.x + cos(angle2) * 500;
		p4.y = origin.y + sin(angle2) * 500;
	}

	auto pBegin = lineIntersection(p3, p4, p1, p2);

	p2.x = origin.x + cos(angle2);
	p2.y = origin.y + sin(angle2);

	auto pEnd = lineIntersection(p3, p4, p1, p2);

	return std::vector<Coord>{pBegin, pEnd};
}

inline std::vector<Coord> calculateVisibility(Coord origin, std::vector<std::shared_ptr<Segment>>& segments,
                                              std::vector<std::shared_ptr<EndPoint>>& endpoints) {
	refreshSegments(origin, segments);

	std::list<std::shared_ptr<Segment>> openSegments;
	std::vector<Coord> polygon;
	double currentAngle = 0.0;

	std::sort(endpoints.begin(), endpoints.end(), endpointCompare);

	for (auto pass = 0; pass < 2; pass++) {
		for (auto endpoint : endpoints) {
			// auto endpoint = endpoints[i];
			auto currentOld = openSegments.empty() ? nullptr : openSegments.front();

			if (endpoint->beginsSegment) {
				auto segmentIter = openSegments.begin();
				while (segmentIter != openSegments.end() && segmentInFrontOf(endpoint->segment, *segmentIter, origin)) {
					segmentIter++;
				}

				if (segmentIter == openSegments.end()) {
					openSegments.push_back(endpoint->segment);
				} else {
					openSegments.insert(segmentIter, endpoint->segment);
				}
			} else {
				openSegments.remove(endpoint->segment);
			}

			auto newOpen = openSegments.empty() ? nullptr : openSegments.front();
			if (currentOld != newOpen) {
				if (pass == 1) {
					auto trianglePoints = getTrianglePoints(origin, currentAngle, endpoint->angle, currentOld);
					polygon.insert(polygon.end(), trianglePoints.begin(), trianglePoints.end());
				}
				currentAngle = endpoint->angle;
			}
		}
	}

	return polygon;
}

// Area2DPolygon compute the area of a 2D polygon
// http://geomalgorithms.com/a01-_area.html#2D%20Polygons
//  Input:  std::vector<Coord>& points = an array of n vertex points
//  Return: the (double) area of the polygon
inline double getPolygonArea(std::vector<Coord>& points) {
	auto n = points.size();

	if (n < 3) {
		return 0;
	}  // a degenerate polygon

	points.push_back(points[0]);

	auto area = 0.0;

	for (unsigned long i = 0; i < n; i++) {
		area += points[i].x * (points[i + 1].y - points[i - 1].y);
	}
	area += points[n].x * (points[1].y - points[n - 1].y);  // wrap-around term
	return area / 2.0;
}