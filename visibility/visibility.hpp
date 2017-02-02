#pragma once

#include <algorithm>
#include <cmath>
#include <list>
#include <vector>

#include "../geom/Coord.hpp"
#include "./loadMap.hpp"
#include "./segmentInFrontOf.hpp"

bool endpointCompare(const std::shared_ptr<EndPoint>& pointA, const std::shared_ptr<EndPoint>& pointB) {
	if (pointA->angle > pointB->angle) return true;
	if (pointA->angle < pointB->angle) return false;
	if (!pointA->beginsSegment && pointB->beginsSegment) return true;
	if (pointA->beginsSegment && !pointB->beginsSegment) return false;
	return false;
}

Coord lineIntersection(Coord point1, Coord point2, Coord point3, Coord point4) {
	auto s = ((point4.x - point3.x) * (point1.y - point3.y) - (point4.y - point3.y) * (point1.x - point3.x)) /
	         ((point4.y - point3.y) * (point2.x - point1.x) - (point4.x - point3.x) * (point2.y - point1.y));

	return Coord(point1.x + s * (point2.x - point1.x), point1.y + s * (point2.y - point1.y));
}

std::vector<Coord> getTrianglePoints(Coord origin, double angle1, double angle2, std::shared_ptr<Segment> segment) {
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

std::vector<Coord> calculateVisibility(Coord origin, std::vector<std::shared_ptr<Segment>>& segments,
                                       std::vector<std::shared_ptr<EndPoint>>& endpoints) {
	refreshSegments(origin, segments);

	std::list<std::shared_ptr<Segment>> openSegments;
	std::vector<Coord> polygon;
	double beginAngle = 0.0;

	std::sort(endpoints.begin(), endpoints.end(), endpointCompare);

	for (auto pass = 0; pass < 2; pass++) {
		for (auto endpoint : endpoints) {
			// auto endpoint = endpoints[i];
			auto openSegment = openSegments.empty() ? nullptr : openSegments.front();

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

			if (openSegment != openSegments.front()) {
				if (pass == 1) {
					auto trianglePoints = getTrianglePoints(origin, beginAngle, endpoint->angle, openSegment);
					polygon.insert(polygon.begin(), trianglePoints.begin(), trianglePoints.end());
				}
				beginAngle = endpoint->angle;
			}
		}
	}

	return polygon;
}