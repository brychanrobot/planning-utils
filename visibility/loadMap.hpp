#pragma once

#include <cmath>
#include <cstdlib>
#include "../geom/Coord.hpp"
#include "../geom/Rect.hpp"

#include "EndPoint.hpp"
#include "Segment.hpp"

inline void segmentsFromRectangle(std::shared_ptr<Rect> r, std::vector<std::shared_ptr<Segment>>& segments) {
	std::vector<Coord> points;
	r->getPoints(points);

	segments.push_back(createSegment(points[0], points[1]));
	segments.push_back(createSegment(points[1], points[2]));
	segments.push_back(createSegment(points[2], points[3]));
	segments.push_back(createSegment(points[3], points[0]));
}

inline void calculateEndPointAngles(Coord origin, std::shared_ptr<Segment> segment) {
	auto dx = 0.5 * (segment->p1->x + segment->p2->x) - origin.x;
	auto dy = 0.5 * (segment->p1->y + segment->p2->y) - origin.y;

	segment->d = dx * dx + dy * dy;
	segment->p1->angle = atan2(segment->p1->y - origin.y, segment->p1->x - origin.x);
	segment->p2->angle = atan2(segment->p2->y - origin.y, segment->p2->x - origin.x);
}

inline void setSegmentBeginning(std::shared_ptr<Segment> segment) {
	auto dAngle = segment->p2->angle - segment->p1->angle;

	if (dAngle <= -M_PI) {
		dAngle += 2 * M_PI;
	} else {
		dAngle -= 2 * M_PI;
	}

	segment->p1->beginsSegment = dAngle > 0;
	segment->p2->beginsSegment = !segment->p1->beginsSegment;
}

inline void refreshSegments(Coord origin, std::vector<std::shared_ptr<Segment>>& segments) {
	for (auto segment : segments) {
		calculateEndPointAngles(origin, segment);
		setSegmentBeginning(segment);
	}
}

inline void loadMap(Rect room, std::vector<std::shared_ptr<Rect>>* obstacles, std::vector<std::shared_ptr<Segment>>& segments,
                    std::vector<std::shared_ptr<EndPoint>>& endpoints) {
	segmentsFromRectangle(std::shared_ptr<Rect>(&room), segments);
	for (auto obstacle : *obstacles) {
		segmentsFromRectangle(obstacle, segments);
	}

	// refreshSegments(origin, segments);

	for (auto segment : segments) {
		endpoints.push_back(segment->p1);
		endpoints.push_back(segment->p2);
	}

	for (auto endpoint : endpoints) {
		printf("%.2f, %.2f\n", endpoint->y, endpoint->y);
	}
}